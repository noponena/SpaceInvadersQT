#ifndef GAMERUNNERVIEW_H
#define GAMERUNNERVIEW_H

#include "Game/CollisionDetection/CollisionDetector.h"
#include "Game/Core/GameHUD.h"
#include "Game/Core/GameState.h"
#include "Game/Levels/LevelManager.h"
#include "UI/FPSCounter.h"
#include "UI/GameObjectCounter.h"
#include <QElapsedTimer>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLabel>
#include <QThread>
#include <QTimer>
#include <QWheelEvent>

namespace Game {
namespace Core {

class GameRunnerView : public QGraphicsView {
  Q_OBJECT
public:
  explicit GameRunnerView(QRect screenGeometry, QWidget *parent = nullptr);
  ~GameRunnerView();
  void startLevel(const Levels::Level &level);
  void quitLevel();
  void resumeGame();

  GameObjects::Ships::PlayerShip *playerShip() const;

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override { event->ignore(); }

private:
  GameState *m_gameState;
  Core::GameHUD *m_gameHUD;
  std::unique_ptr<Levels::LevelManager> m_levelManager;
  QTimer m_gameTimer;
  QTimer m_benchmarkTimer;
  GameObjects::Ships::PlayerShip *m_playerShip;
  std::unique_ptr<CollisionDetection::CollisionDetector> m_collisionDetector;
  QElapsedTimer m_elapsedTimer;
  QElapsedTimer m_fpsTimer;
  QSet<int> m_pressedKeys;
  QGraphicsScene m_scene;
  int m_frameCount = 0;
  QGraphicsTextItem *m_stellarTokens;
  QGraphicsTextItem *m_playerHp;
  QGraphicsTextItem *m_levelEndedInfo;
  bool m_continuousShoot;
  bool m_continuousEnemySpawn;
  bool m_levelFailed;
  bool m_levelFailedOrPassedInfoDisplayed;
  bool m_spawnEventsFinished;
  std::chrono::high_resolution_clock::time_point m_lastFrameEndTime;

  UI::FPSCounter *m_fpsCounter;
  UI::GameObjectCounter *m_gameObjectCounter;

  void setupView();
  void setupCounters();
  void setupConnections();
  inline void gameLoop();
  void initializeBenchmark();

  inline void processInput(float deltaTimeInSeconds);
  inline void processGameAction(float deltaTimeInSeconds);
  inline void processMenuAction();
  inline void updateGameState(float deltaTime);
  inline void updateFps();
  inline void displayLevelFailedInfo();
  inline void displayLevelPassedInfo();

  const std::vector<std::shared_ptr<GameObjects::GameObject>> *m_gameObjects;

  using MenuAction = std::function<void()>;
  using GameAction = std::function<void(float)>;

  const std::unordered_map<int, MenuAction> m_menuActions{
      {Qt::Key_Escape,
       [&]() {
         m_pressedKeys.remove(Qt::Key_Escape);
         pause();
       }},
  };
  const std::unordered_map<int, GameAction> m_gameActions{
      {Qt::Key_Left, [&](float dt) { m_playerShip->accelerateLeft(dt); }},
      {Qt::Key_Right, [&](float dt) { m_playerShip->accelerateRight(dt); }},
      {Qt::Key_Down, [&](float dt) { m_playerShip->accelerateDown(dt); }},
      {Qt::Key_Up, [&](float dt) { m_playerShip->accelerateUp(dt); }},
      {Qt::Key_Space,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->firePrimaryWeapons();
       }},
      {Qt::Key_1,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->fireSecondaryWeapon(0);
       }},
      {Qt::Key_2,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->fireSecondaryWeapon(1);
       }},
      {Qt::Key_P,
       [&](float dt) {
         Q_UNUSED(dt);
         m_playerShip->takeDamage(1);
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
         m_playerShip->setAutoShoot(m_continuousShoot);
         m_pressedKeys.remove(Qt::Key_C);
       }},
      {Qt::Key_S,
       [&](float dt) {
         Q_UNUSED(dt);
         m_continuousEnemySpawn = !m_continuousEnemySpawn;
         m_pressedKeys.remove(Qt::Key_S);
       }},
  };

  inline float calculateRenderTime(
      const std::chrono::high_resolution_clock::time_point &loopStartTime);
  inline float calculateDeltaTime();
  inline void manageEnemySpawn(float deltaTimeInSeconds);
  template <typename Func> inline float measureFunctionDuration(Func &&func);
  inline void logFrameStatistics(float renderTimeUs, float updateTimeUs,
                                 float collisionDetectionTimeUs);
  inline void updateGameCounters();
  inline void checkLevelFailedOrPassed();
signals:
  void fpsUpdated(int fps);
  void gamePaused();
  void levelQuit();

private slots:
  void onObjectAdded(QGraphicsItem *object) { m_scene.addItem(object); }
  void onObjectDeleted(QGraphicsItem *object) { m_scene.removeItem(object); }
  void onPlayerShipDestroyed() {
    m_levelFailed = true;
    m_playerShip = nullptr;
  }
  void onEnemyLimitReached() {
    m_levelFailed = true;
    m_playerShip = nullptr;
  }
  void onSpawnEventsFinished() { m_spawnEventsFinished = true; }
  void pause() {
    m_gameTimer.stop();
    emit gamePaused();
  }
};
} // namespace Core
} // namespace Game

#endif // GAMERUNNERVIEW_H
