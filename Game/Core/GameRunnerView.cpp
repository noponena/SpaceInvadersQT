#include "Game/Core/GameRunnerView.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Utils/PerformanceBenchmark.h"
#include <QOpenGLWidget>
#include <QTimer>

namespace Game {
namespace Core {
GameRunnerView::GameRunnerView(QRect screenGeometry, QWidget *parent)
    : QGraphicsView(parent), m_scene(this), m_continuousShoot(false),
      m_continuousEnemySpawn(true), m_levelFailed(false),
      m_levelFailedOrPassedInfoDisplayed(false), m_spawnEventsFinished(false) {
  m_gameState = new GameState();
  m_levelManager = std::make_unique<Levels::LevelManager>(m_gameState);
  m_gameObjects = &(m_gameState->gameObjects());
  m_collisionDetector = std::make_unique<CollisionDetection::CollisionDetector>(
      m_gameState->gameObjects(), screenGeometry);
  setupView();
  scene()->setSceneRect(screenGeometry);
  setupCounters();
  m_elapsedTimer.start();
  m_fpsTimer.start();

  m_gameHUD =
      new Core::GameHUD(screenGeometry.width(), screenGeometry.height());
  m_scene.addItem(m_gameHUD);
  m_gameHUD->setPos(0, screenGeometry.height() * 0.9);

  setupConnections();

  m_gameState->setSize(screenGeometry.width(), screenGeometry.height());
}

GameRunnerView::~GameRunnerView() {
  // We have to make sure that the game objects
  // are destroyed before the scene (QGraphicsScene)
  // destroys the graphics items in the scene.
  delete m_gameState;
}

void GameRunnerView::setupView() {
  QSurfaceFormat format;
  // format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
  format.setSwapInterval(0);
  format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
  QOpenGLWidget *glWidget = new QOpenGLWidget;
  glWidget->setFormat(format);
  QSurfaceFormat::setDefaultFormat(format);
  setViewport(glWidget);

  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setAlignment(Qt::AlignTop);
  setInteractive(false);
  setRenderHints(QPainter::SmoothPixmapTransform);
  setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing |
                       QGraphicsView::DontSavePainterState);
  setViewportMargins(0, 0, 0, 0);
  setScene(&m_scene);
  setStyleSheet("border:0px");
  m_scene.setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  m_scene.setBackgroundBrush(QBrush(Qt::black));
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameRunnerView::setupCounters() {
  m_fpsCounter = new UI::FPSCounter();
  m_gameObjectCounter = new UI::GameObjectCounter();

  m_stellarTokens = new QGraphicsTextItem();
  m_stellarTokens->setPlainText("Stellar tokens: 0");
  m_stellarTokens->setDefaultTextColor(Qt::white);
  m_stellarTokens->setFont(QFont("times", 12));

  m_playerHp = new QGraphicsTextItem();
  m_playerHp->setDefaultTextColor(Qt::white);
  m_playerHp->setFont(QFont("times", 12));

  m_levelEndedInfo = new QGraphicsTextItem();
  m_levelEndedInfo->setDefaultTextColor(Qt::white);
  m_levelEndedInfo->setFont(QFont("times", 64));

  m_fpsCounter->setPos(0, 0);
  m_gameObjectCounter->setPos(0, m_fpsCounter->boundingRect().height() - 10);
  m_stellarTokens->setPos(0, m_fpsCounter->boundingRect().height() - 10 + 20);
  m_playerHp->setPos(0, m_gameObjectCounter->boundingRect().height() - 10 + 40);

  scene()->addItem(m_fpsCounter);
  scene()->addItem(m_gameObjectCounter);
  scene()->addItem(m_stellarTokens);
  scene()->addItem(m_playerHp);
  connect(this, &GameRunnerView::fpsUpdated, m_fpsCounter,
          &UI::FPSCounter::updateFPS);
}

void GameRunnerView::setupConnections() {
  connect(m_gameState, &GameState::objectAdded, this,
          &GameRunnerView::onObjectAdded);
  connect(m_gameState, &GameState::objectDeleted, this,
          &GameRunnerView::onObjectDeleted);
  connect(m_gameState, &GameState::playerShipDestroyed, this,
          &GameRunnerView::onPlayerShipDestroyed);

  connect(&m_gameTimer, &QTimer::timeout, this, &GameRunnerView::gameLoop);
  connect(m_levelManager.get(), &Levels::LevelManager::enemyLimitReached, this,
          &GameRunnerView::onEnemyLimitReached);
  connect(m_levelManager.get(), &Levels::LevelManager::spawnEventsFinished,
          this, &GameRunnerView::onSpawnEventsFinished);
}

void GameRunnerView::startLevel(const Levels::Level &level) {
  qDebug() << "starting game..";
  bool perfTest = false;

#ifdef PERFORMANCE_BENCHMARK
  perfTest = true;
  initializeBenchmark();
#endif
  // m_levelManager = std::make_unique<LevelManager>(m_gameState, perfTest);
  m_gameState->createPlayerShip();
  m_playerShip = m_gameState->playerShip();

  connect(m_playerShip,
          &GameObjects::Ships::PlayerShip::playerSecondaryWeaponsChanged,
          m_gameHUD, &Core::GameHUD::onPlayerSecondaryWeaponsChanged);

  connect(m_playerShip,
          &GameObjects::Ships::PlayerShip::playerSecondaryWeaponFired,
          m_gameHUD, &Core::GameHUD::onPlayerSecondaryWeaponFired);

  connect(m_playerShip, &GameObjects::Ships::PlayerShip::playerEnergyUpdated,
          m_gameHUD, &Core::GameHUD::onPlayerEnergyUpdated);

  connect(m_playerShip, &GameObjects::Ships::PlayerShip::playerMaxEnergySet,
          m_gameHUD, &Core::GameHUD::onPlayerMaxEnergySet);

  connect(m_playerShip, &GameObjects::Ships::PlayerShip::playerHealthUpdated,
          m_gameHUD, &Core::GameHUD::onPlayerHealthUpdated);

  connect(m_playerShip, &GameObjects::Ships::PlayerShip::playerMaxHealthSet,
          m_gameHUD, &Core::GameHUD::onPlayerMaxHealthSet);

  m_gameState->initialize();
  m_levelManager->setLevel(level);
  m_levelManager->startLevel();
  m_gameTimer.start(0);
}

void GameRunnerView::quitLevel() {
  m_gameTimer.stop();
  m_levelFailed = false;
  m_spawnEventsFinished = false;
  if (m_levelFailedOrPassedInfoDisplayed) {
    scene()->removeItem(m_levelEndedInfo);
  }
  m_levelFailedOrPassedInfoDisplayed = false;
  m_gameState->deinitialize();
}

void GameRunnerView::resumeGame() {
  qDebug() << "resuming game..";
  m_elapsedTimer.start();
  m_gameTimer.start(0);
}

void GameRunnerView::gameLoop() {
  auto loopStartTime = std::chrono::high_resolution_clock::now();

  float renderTimeUs = calculateRenderTime(loopStartTime);
  float deltaTimeInSeconds = calculateDeltaTime();
  manageEnemySpawn(deltaTimeInSeconds);

#ifndef PERFORMANCE_BENCHMARK
  processInput(deltaTimeInSeconds);
#endif

  float updateTimeUs =
      measureFunctionDuration([&]() { updateGameState(deltaTimeInSeconds); });
  float collisionDetectionTimeUs =
      measureFunctionDuration([&]() { m_collisionDetector->detectBVH(); });
  updateFps();

  // logFrameStatistics(renderTimeUs, updateTimeUs, collisionDetectionTimeUs);

  updateGameCounters();
  checkLevelFailedOrPassed();
  m_lastFrameEndTime = std::chrono::high_resolution_clock::now();
}

float GameRunnerView::calculateRenderTime(
    const std::chrono::high_resolution_clock::time_point &loopStartTime) {
  if (m_lastFrameEndTime.time_since_epoch().count() > 0) {
    auto renderDuration = std::chrono::duration_cast<std::chrono::microseconds>(
        loopStartTime - m_lastFrameEndTime);
    return renderDuration.count();
  }
  return 0;
}

float GameRunnerView::calculateDeltaTime() {
  int frameTimeMs = m_elapsedTimer.restart();
#ifdef PERFORMANCE_BENCHMARK
  Utils::PerformanceBenchmark::getInstance().logPerformance(
      frameTimeMs, m_gameObjects->size(), m_scene.items().size());
#endif
  return static_cast<float>(frameTimeMs) / 1000.0f;
}

void GameRunnerView::manageEnemySpawn(float deltaTimeInSeconds) {
  int timeToSleep = deltaTimeInSeconds > 0.005f
                        ? 0
                        : 5 - static_cast<int>(deltaTimeInSeconds * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
  if (m_continuousEnemySpawn) {
    m_levelManager->progressLevel();
  }
}

template <typename Func>
float GameRunnerView::measureFunctionDuration(Func &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start)
      .count();
}

void GameRunnerView::logFrameStatistics(float renderTimeUs, float updateTimeUs,
                                        float collisionDetectionTimeUs) {
  if (renderTimeUs > 0) {
    float totalTimeUs = renderTimeUs + updateTimeUs + collisionDetectionTimeUs;

    QString renderPercent =
        QString::number((renderTimeUs / totalTimeUs) * 100.0f, 'f', 2);
    QString updatePercent =
        QString::number((updateTimeUs / totalTimeUs) * 100.0f, 'f', 2);
    QString collisionPercent = QString::number(
        (collisionDetectionTimeUs / totalTimeUs) * 100.0f, 'f', 2);

    qDebug() << "render:" << renderPercent + "%"
             << "update:" << updatePercent + "%"
             << "collision:" << collisionPercent + "%";
  }
}

void GameRunnerView::updateGameCounters() {
  m_gameObjectCounter->setObjectCount(m_gameObjects->size());

  if (!m_levelFailed) {
    int playerHp = m_playerShip->currentHp();
    m_stellarTokens->setPlainText(
        "Stellar tokens: " + QString::number(m_gameState->stellarTokens()));
    m_playerHp->setPlainText("Player HP: " + QString::number(playerHp));
  }
}

void GameRunnerView::checkLevelFailedOrPassed() {
  if (!m_levelFailedOrPassedInfoDisplayed) {
    if (m_levelFailed)
      displayLevelFailedInfo();
    else if (m_spawnEventsFinished && m_gameState->enemyShipCount() <= 0)
      displayLevelPassedInfo();
  }
}

void GameRunnerView::initializeBenchmark() {
  Utils::PerformanceBenchmark::getInstance().initializeBenchmark(m_playerShip);
  connect(&m_benchmarkTimer, &QTimer::timeout, this, ([]() {
    Utils::PerformanceBenchmark::getInstance().logPerformanceScore();
  }));
  connect(&m_benchmarkTimer, &QTimer::timeout, this, &GameRunnerView::pause);
  m_benchmarkTimer.start(30000);
}

void GameRunnerView::processInput(float deltaTimeInSeconds) {

  if (!m_levelFailed) {
    processGameAction(deltaTimeInSeconds);
  }

  processMenuAction();
}

void GameRunnerView::processGameAction(float deltaTimeInSeconds) {
  for (const auto &[key, action] : m_gameActions) {
    if (m_pressedKeys.contains(key))
      action(deltaTimeInSeconds);
  }

  if ((!m_pressedKeys.contains(Qt::Key_Left) &&
       !m_pressedKeys.contains(Qt::Key_Right)) ||
      (m_pressedKeys.contains(Qt::Key_Left) &&
       m_pressedKeys.contains(Qt::Key_Right))) {
    m_playerShip->decelerateX(deltaTimeInSeconds);
  }

  if ((!m_pressedKeys.contains(Qt::Key_Up) &&
       !m_pressedKeys.contains(Qt::Key_Down)) ||
      (m_pressedKeys.contains(Qt::Key_Up) &&
       m_pressedKeys.contains(Qt::Key_Down))) {
    m_playerShip->decelerateY(deltaTimeInSeconds);
  }

  m_playerShip->moveHorizontal(deltaTimeInSeconds);
  m_playerShip->moveVertical(deltaTimeInSeconds);
}

void GameRunnerView::processMenuAction() {
  for (const auto &[key, action] : m_menuActions) {
    if (m_pressedKeys.contains(key))
      action();
  }
}

void GameRunnerView::updateGameState(float deltaTime) {
  m_gameState->update(deltaTime);
}

void GameRunnerView::updateFps() {
  m_frameCount++;
  if (m_fpsTimer.elapsed() >= 1000) {
    emit fpsUpdated(m_frameCount);
    m_frameCount = 0;
    m_fpsTimer.restart();
  }
}

void GameRunnerView::displayLevelFailedInfo() {
  m_levelEndedInfo->setPlainText("LEVEL FAILED");
  QRectF textBoundingRect = m_levelEndedInfo->boundingRect();
  QRectF sceneRect = scene()->sceneRect();
  QPointF centerPosition =
      QPointF((sceneRect.width() - textBoundingRect.width()) / 2.0,
              (sceneRect.height() - textBoundingRect.height()) / 2.0);
  m_levelEndedInfo->setPos(centerPosition);
  m_levelFailedOrPassedInfoDisplayed = true;
  scene()->addItem(m_levelEndedInfo);
}

void GameRunnerView::displayLevelPassedInfo() {
  m_levelEndedInfo->setPlainText("LEVEL SUCCESSFUL");
  QRectF textBoundingRect = m_levelEndedInfo->boundingRect();
  QRectF sceneRect = scene()->sceneRect();
  QPointF centerPosition =
      QPointF((sceneRect.width() - textBoundingRect.width()) / 2.0,
              (sceneRect.height() - textBoundingRect.height()) / 2.0);
  m_levelEndedInfo->setPos(centerPosition);
  m_levelFailedOrPassedInfoDisplayed = true;
  scene()->addItem(m_levelEndedInfo);
}

void GameRunnerView::keyPressEvent(QKeyEvent *event) {
  m_pressedKeys.insert(event->key());
}

void GameRunnerView::keyReleaseEvent(QKeyEvent *event) {
  m_pressedKeys.remove(event->key());
}

GameObjects::Ships::PlayerShip *GameRunnerView::playerShip() const {
  return m_playerShip;
}

} // namespace Core
} // namespace Game
