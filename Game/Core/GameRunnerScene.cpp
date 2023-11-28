#include "Game/Core/GameRunnerScene.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Utils/PerformanceBenchmark.h"
#include <QOpenGLWidget>
#include <QTimer>

namespace Game {
namespace Core {
GameRunnerScene::GameRunnerScene(QWidget *parent)
    : QGraphicsView(parent), m_scene(this),
      m_continuousShoot(false), m_continuousEnemySpawn(true), m_gameOver(false),
      m_gameOverInfoDisplayed(false) {
  m_gameState = new GameState();
  setupView();
  setupCounters();
  setupConnections();
  m_elapsedTimer.start();
  m_fpsTimer.start();
}

GameRunnerScene::~GameRunnerScene() {
  // We have to make sure that the game objects
  // are destroyed before the scene (QGraphicsScene)
  // destroys the graphics items in the scene.
  delete m_gameState;
}

void GameRunnerScene::setupView() {
  QSurfaceFormat format;
  // format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
  format.setSwapInterval(0);
  format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
  QOpenGLWidget *glWidget = new QOpenGLWidget;
  glWidget->setFormat(format);
  QSurfaceFormat::setDefaultFormat(format);
  setViewport(glWidget);

  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setAlignment(Qt::AlignCenter);
  setInteractive(false);
  setRenderHints(QPainter::SmoothPixmapTransform);
  setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing |
                       QGraphicsView::DontSavePainterState);
  setViewportMargins(0, 0, 0, 0);
  setScene(&m_scene);
  m_scene.setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  m_scene.setBackgroundBrush(QBrush(Qt::black));
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameRunnerScene::setupCounters() {
  m_fpsCounter = new UI::FPSCounter();
  m_gameObjectCounter = new UI::GameObjectCounter();

  m_sceneItemCounter = new QGraphicsTextItem();
  m_sceneItemCounter->setDefaultTextColor(Qt::white);
  m_sceneItemCounter->setFont(QFont("times", 12));

  m_stellarTokens = new QGraphicsTextItem();
  m_stellarTokens->setPlainText("Stellar tokens: 0");
  m_stellarTokens->setDefaultTextColor(Qt::white);
  m_stellarTokens->setFont(QFont("times", 12));

  m_playerHp = new QGraphicsTextItem();
  m_playerHp->setDefaultTextColor(Qt::white);
  m_playerHp->setFont(QFont("times", 12));

  m_gameOverInfo = new QGraphicsTextItem();
  m_gameOverInfo->setDefaultTextColor(Qt::white);
  m_gameOverInfo->setFont(QFont("times", 64));

  m_fpsCounter->setPos(0, 0);
  m_gameObjectCounter->setPos(0, m_fpsCounter->boundingRect().height() - 10);
  m_sceneItemCounter->setPos(0,
                             m_fpsCounter->boundingRect().height() - 10 + 20);
  m_stellarTokens->setPos(0, m_gameObjectCounter->boundingRect().height() - 10 +
                                 40);
  m_playerHp->setPos(0, m_gameObjectCounter->boundingRect().height() - 10 + 60);

  scene()->addItem(m_fpsCounter);
  scene()->addItem(m_gameObjectCounter);
  scene()->addItem(m_sceneItemCounter);
  scene()->addItem(m_stellarTokens);
  scene()->addItem(m_playerHp);
  scene()->addItem(m_gameOverInfo);
  connect(this, &GameRunnerScene::fpsUpdated, m_fpsCounter,
          &UI::FPSCounter::updateFPS);
}

void GameRunnerScene::setupConnections() {
  connect(m_gameState, &GameState::objectAdded, this,
          &GameRunnerScene::onObjectAdded);
  connect(m_gameState, &GameState::objectDeleted, this,
          &GameRunnerScene::onObjectDeleted);
  connect(m_gameState, &GameState::playerShipDestroyed, this,
          &GameRunnerScene::onPlayerShipDestroyed);
}

void GameRunnerScene::startGame() {
  // Initialize game state
  qDebug() << "starting game..";
  m_gameState->setSize(scene()->sceneRect().width(),
                       scene()->sceneRect().height());
  m_gameState->initialize();
  m_playerShip = m_gameState->playerShip();
  m_gameObjects = &(m_gameState->gameObjects());
  m_collisionDetector = std::make_unique<CollisionDetection::CollisionDetector>(
      m_gameState->gameObjects(), rect());

  // Create and start game loop timer

  connect(&m_gameTimer, &QTimer::timeout, this, &GameRunnerScene::gameLoop);
  bool perfTest = false;

#ifdef PERFORMANCE_BENCHMARK
  perfTest = true;
  initializeBenchmark();
#endif
  m_levelManager = std::make_unique<LevelManager>(m_gameState, perfTest);
  m_gameTimer.start(0);

  // Hide menu and show game window
  // ...
}

void GameRunnerScene::gameLoop() {
  auto loopStartTime = std::chrono::high_resolution_clock::now();

  float renderTimeUs = calculateRenderTime(loopStartTime);
  float deltaTimeInSeconds = calculateDeltaTime();
  manageEnemySpawn(deltaTimeInSeconds);

#ifndef PERFORMANCE_BENCHMARK
  processInput(deltaTimeInSeconds);
#endif

  float updateTimeUs = measureFunctionDuration([&]() { updateGameState(deltaTimeInSeconds); });
  float collisionDetectionTimeUs = measureFunctionDuration([&]() { m_collisionDetector->detectBVH(); });
  updateFps();

  logFrameStatistics(renderTimeUs, updateTimeUs, collisionDetectionTimeUs);

  updateGameCounters();
  checkGameOver();

  m_lastFrameEndTime = std::chrono::high_resolution_clock::now();
}

float GameRunnerScene::calculateRenderTime(const std::chrono::high_resolution_clock::time_point& loopStartTime) {
  if (m_lastFrameEndTime.time_since_epoch().count() > 0) {
      auto renderDuration = std::chrono::duration_cast<std::chrono::microseconds>(loopStartTime - m_lastFrameEndTime);
      return renderDuration.count();
  }
  return 0;
}

float GameRunnerScene::calculateDeltaTime() {
  int frameTimeMs = m_elapsedTimer.restart();
  return static_cast<float>(frameTimeMs) / 1000.0f;
}

void GameRunnerScene::manageEnemySpawn(float deltaTimeInSeconds) {
  int timeToSleep = deltaTimeInSeconds > 0.005f ? 0 : 5 - static_cast<int>(deltaTimeInSeconds * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(timeToSleep));
  if (m_continuousEnemySpawn) {
      m_levelManager->update();
  }
}

template<typename Func>
float GameRunnerScene::measureFunctionDuration(Func&& func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

void GameRunnerScene::logFrameStatistics(float renderTimeUs, float updateTimeUs, float collisionDetectionTimeUs) {
  if (renderTimeUs > 0) {
      float totalTimeUs = renderTimeUs + updateTimeUs + collisionDetectionTimeUs;

      QString renderPercent = QString::number((renderTimeUs / totalTimeUs) * 100.0f, 'f', 2);
      QString updatePercent = QString::number((updateTimeUs / totalTimeUs) * 100.0f, 'f', 2);
      QString collisionPercent = QString::number((collisionDetectionTimeUs / totalTimeUs) * 100.0f, 'f', 2);

      qDebug() << "render:" << renderPercent + "%"
               << "update:" << updatePercent + "%"
               << "collision:" << collisionPercent + "%";
  }
}


void GameRunnerScene::updateGameCounters() {
  int gameObjectCount = m_gameObjects->size();
  int sceneItemCount = scene()->items().size();

  m_sceneItemCounter->setPlainText("Scene items: " + QString::number(sceneItemCount));
  m_gameObjectCounter->setObjectCount(gameObjectCount);

  if (!m_gameOver) {
      int playerHp = m_playerShip->currentHp();
      m_stellarTokens->setPlainText("Stellar tokens: " + QString::number(m_gameState->stellarTokens()));
      m_playerHp->setPlainText("Player HP: " + QString::number(playerHp));
  }
}

void GameRunnerScene::checkGameOver() {
#ifdef PERFORMANCE_BENCHMARK
  Utils::PerformanceBenchmark::getInstance().logPerformance(
      frameTimeMs, gameObjectCount, sceneItemCount);
#endif

  if (m_gameOver && !m_gameOverInfoDisplayed) {
      displayGameOverInfo();
  }
}

void GameRunnerScene::initializeBenchmark() {
  Utils::PerformanceBenchmark::getInstance().initializeBenchmark(m_playerShip);
  connect(&m_benchmarkTimer, &QTimer::timeout, this, ([]() {
    Utils::PerformanceBenchmark::getInstance().logPerformanceScore();
  }));
  connect(&m_benchmarkTimer, &QTimer::timeout, this, &GameRunnerScene::quit);
  m_benchmarkTimer.start(30000);
}

void GameRunnerScene::processInput(float deltaTimeInSeconds) {

  if (!m_gameOver) {
    processGameAction(deltaTimeInSeconds);
  }

  processMenuAction();
}

void GameRunnerScene::processGameAction(float deltaTimeInSeconds) {
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

void GameRunnerScene::processMenuAction() {
  for (const auto &[key, action] : m_menuActions) {
    if (m_pressedKeys.contains(key))
      action();
  }
}

void GameRunnerScene::updateGameState(float deltaTime) {
  m_gameState->update(deltaTime);
}

void GameRunnerScene::updateFps() {
  m_frameCount++;
  if (m_fpsTimer.elapsed() >= 1000) {
    emit fpsUpdated(m_frameCount);
    m_frameCount = 0;
    m_fpsTimer.restart();
  }
}

void GameRunnerScene::displayGameOverInfo() {
  m_gameOverInfo->setPlainText("GAME OVER");
  QRectF textBoundingRect = m_gameOverInfo->boundingRect();
  QRectF sceneRect = scene()->sceneRect();
  QPointF centerPosition =
      QPointF((sceneRect.width() - textBoundingRect.width()) / 2.0,
              (sceneRect.height() - textBoundingRect.height()) / 2.0);
  m_gameOverInfo->setPos(centerPosition);
  m_gameOverInfoDisplayed = true;
}

void GameRunnerScene::keyPressEvent(QKeyEvent *event) {
  m_pressedKeys.insert(event->key());
}

void GameRunnerScene::keyReleaseEvent(QKeyEvent *event) {
  m_pressedKeys.remove(event->key());
}
} // namespace Core
} // namespace Game
