#include "Game/Core/GameRunner.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Utils/PerformanceMonitor.h"
#include <QOpenGLWidget>
#include <QTimer>

namespace Game {
namespace Core {
GameRunner::GameRunner(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)),
      m_continuousShoot(false), m_continuousEnemySpawn(true), m_gameOver(false),
      m_gameOverInfoDisplayed(false) {
  m_gameState = new GameState();
  setupView();
  setupCounters();
  setupConnections();
  m_elapsedTimer.start();
  m_fpsTimer.start();
}

GameRunner::~GameRunner() {
  // We have to make sure that the game objects
  // are destroyed before the scene (QGraphicsScene)
  // destroys the graphics items in the scene.
  delete m_gameState;
}

void GameRunner::setupView() {
  setViewport(new QOpenGLWidget);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  setAlignment(Qt::AlignCenter);
  setInteractive(false);
  setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing |
                       QGraphicsView::DontSavePainterState);
  setViewportMargins(0, 0, 0, 0);
  setScene(&m_scene);
  m_scene.setItemIndexMethod(QGraphicsScene::BspTreeIndex);
  m_scene.setBackgroundBrush(QBrush(Qt::black));
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameRunner::setupCounters() {
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
  connect(this, &GameRunner::fpsUpdated, m_fpsCounter,
          &UI::FPSCounter::updateFPS);
}

void GameRunner::setupConnections() {
  connect(m_gameState, &GameState::objectAdded, this,
          &GameRunner::onObjectAdded);
  connect(m_gameState, &GameState::objectDeleted, this,
          &GameRunner::onObjectDeleted);
  connect(m_gameState, &GameState::playerShipDestroyed, this,
          &GameRunner::onPlayerShipDestroyed);
}

void GameRunner::startGame() {
  // Initialize game state
  qDebug() << "starting game..";
  m_gameState->setSize(scene()->sceneRect().width(),
                       scene()->sceneRect().height());
  m_gameState->initialize();
  m_playerShip = m_gameState->playerShip();
  m_gameObjects = &(m_gameState->gameObjects());
  m_levelManager = std::make_unique<LevelManager>(m_gameState);
  m_collisionDetector =
      std::make_unique<CollisionDetector>(m_gameState->gameObjects(), rect());

  // Create and start game loop timer

  connect(&m_gameTimer, &QTimer::timeout, this, &GameRunner::gameLoop);
  m_gameTimer.start(8); // Approx. 120 frames per second

  // Hide menu and show game window
  // ...
}

void GameRunner::gameLoop() {
  int frameTimeMs = m_elapsedTimer.restart();
  float deltaTimeInSeconds = static_cast<float>(frameTimeMs) / 1000.0f;
  if (m_continuousEnemySpawn)
    m_levelManager->update();
  if (m_continuousShoot)
    m_playerShip->shoot();

  processInput(deltaTimeInSeconds);
  updateGameState(deltaTimeInSeconds);
  m_collisionDetector->detectQuadTree();
  updateFps();

  int gameObjectCount = m_gameObjects->size();
  int sceneItemCount = scene()->items().size();

  m_sceneItemCounter->setPlainText("Scene items: " +
                                   QString::number(sceneItemCount));
  m_gameObjectCounter->setObjectCount(gameObjectCount);

  if (!m_gameOver) {
    int playerHp = m_playerShip->currentHp();
    m_stellarTokens->setPlainText(
        "Stellar tokens: " + QString::number(m_gameState->stellarTokens()));
    m_playerHp->setPlainText("Player HP: " + QString::number(playerHp));
  }

#ifdef LOG_PERFORMANCE
  Utils::PerformanceMonitor::getInstance().logPerformance(
      frameTimeMs, gameObjectCount, sceneItemCount);
#endif

  if (m_gameOver && !m_gameOverInfoDisplayed) {
    displayGameOverInfo();
  }
}

void GameRunner::processInput(float deltaTimeInSeconds) {

  if (!m_gameOver) {
    processGameAction(deltaTimeInSeconds);
  }

  processMenuAction();
}

void GameRunner::processGameAction(float deltaTimeInSeconds) {
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

void GameRunner::processMenuAction() {
  for (const auto &[key, action] : m_menuActions) {
    if (m_pressedKeys.contains(key))
      action();
  }
}

void GameRunner::updateGameState(float deltaTime) {
  m_gameState->update(deltaTime);
}

void GameRunner::updateFps() {
  m_frameCount++;
  if (m_fpsTimer.elapsed() >= 1000) {
    emit fpsUpdated(m_frameCount);
    m_frameCount = 0;
    m_fpsTimer.restart();
  }
}

void GameRunner::displayGameOverInfo() {
  m_gameOverInfo->setPlainText("GAME OVER");
  QRectF textBoundingRect = m_gameOverInfo->boundingRect();
  QRectF sceneRect = scene()->sceneRect();
  QPointF centerPosition =
      QPointF((sceneRect.width() - textBoundingRect.width()) / 2.0,
              (sceneRect.height() - textBoundingRect.height()) / 2.0);
  m_gameOverInfo->setPos(centerPosition);
  m_gameOverInfoDisplayed = true;
}

void GameRunner::keyPressEvent(QKeyEvent *event) {
  m_pressedKeys.insert(event->key());
}

void GameRunner::keyReleaseEvent(QKeyEvent *event) {
  m_pressedKeys.remove(event->key());
}
} // namespace Core
} // namespace Game
