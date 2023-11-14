#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#include "GameState.h"
#include "LevelManager.h"
#include "CollisionDetection/CollisionDetector.h"
#include "UI/FPSCounter.h"
#include "UI/GameObjectCounter.h"
#include "qtimer.h"
#include <QElapsedTimer>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QLabel>

namespace Game {
class GameRunner : public QGraphicsView {
  Q_OBJECT
public:
  explicit GameRunner(QWidget *parent = nullptr);
  void startGame();

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override { event->ignore(); }

private:
  QTimer *m_gameTimer;
  std::unique_ptr<LevelManager> m_levelManager;
  GameObjects::Ships::PlayerShip *m_playerShip;
  CollisionDetector *m_collisionDetector;
  QElapsedTimer m_elapsedTimer;
  QElapsedTimer m_fpsTimer;
  Game::GameState m_gameState;
  QSet<int> m_pressedKeys;
  QGraphicsScene m_scene;
  int m_frameCount = 0;
  QGraphicsTextItem* m_stellarTokens;
  QGraphicsTextItem* m_playerHp;
  QGraphicsTextItem* m_gameOverInfo;
  QGraphicsTextItem* m_sceneItemCounter;
  bool m_continuousShoot;
  bool m_continuousEnemySpawn;
  bool m_gameOver;
  bool m_gameOverInfoDisplayed;

  UI::FPSCounter *m_fpsCounter;
  UI::GameObjectCounter *m_gameObjectCounter;

  void setupView();
  void setupCounters();
  void setupConnections();
  void gameLoop();

  inline void processInput(float deltaTimeInSeconds);
  inline void processGameAction(float deltaTimeInSeconds);
  inline void processMenuAction();
  inline void updateGameState(float deltaTime);
  inline void updateFps();
  inline void displayGameOverInfo();

  const std::list<std::shared_ptr<GameObjects::GameObject>> *m_gameObjects;

  using MenuAction = std::function<void()>;
  using GameAction = std::function<void(float)>;

  const std::unordered_map<int, MenuAction> m_menuActions {
      {Qt::Key_Escape, [&]() { m_gameTimer->stop(); emit windowClosed(); }},
  };
  const std::unordered_map<int, GameAction> m_gameActions{
      {Qt::Key_Left, [&](float dt) { m_playerShip->accelerateLeft(dt); }},
      {Qt::Key_Right, [&](float dt) { m_playerShip->accelerateRight(dt); }},
      {Qt::Key_Down, [&](float dt) { m_playerShip->accelerateDown(dt); }},
      {Qt::Key_Up, [&](float dt) { m_playerShip->accelerateUp(dt); }},
      {Qt::Key_Space,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->shoot();
       }},
      {Qt::Key_Q,
       [&](float dt) {
         Q_UNUSED(dt);
         m_gameState.initEnemyShips();
       }},
      {Qt::Key_U,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->updateFireRate(-10);
       }},
      {Qt::Key_D,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->updateFireRate(10);
       }},
      {Qt::Key_C,
       [&](float dt) {
           Q_UNUSED(dt);
           m_continuousShoot = !m_continuousShoot;
           m_pressedKeys.remove(Qt::Key_C);
       }},
      {Qt::Key_S,
       [&](float dt) {
           Q_UNUSED(dt);
           m_continuousEnemySpawn = !m_continuousEnemySpawn;
           m_pressedKeys.remove(Qt::Key_S);
       }},
  };

signals:
  void fpsUpdated(int fps);
  void windowClosed();

private slots:
  void onObjectAdded(QGraphicsItem *object) {
    m_scene.addItem(object);
    m_gameObjectCounter->updateObjectCount(1);
  }
  void onObjectDeleted(QGraphicsItem *object) {
    m_scene.removeItem(object);
    m_gameObjectCounter->updateObjectCount(-1);
  }
  void onPlayerShipDestroyed() {
    m_gameOver = true;
    m_playerShip = nullptr;
  }
};
} // namespace Game

#endif // GAMERUNNER_H
