#ifndef GAMERUNNERVIEW_H
#define GAMERUNNERVIEW_H

#include <QElapsedTimer>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLabel>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QThread>
#include <QTimer>
#include <QWheelEvent>
#include "Game/CollisionDetection/CollisionDetector.h"
#include "Game/Core/GameState.h"
#include "Game/Levels/LevelManager.h"
#include "UI/FPSCounter.h"
#include "UI/GLProgressBar.h"
#include "UI/GameObjectCounter.h"
#include "UI/Panel.h"

namespace Game {
namespace Core {

class GameRunnerView : public QOpenGLWidget, QOpenGLFunctions_3_3_Core {
  Q_OBJECT
public:
  explicit GameRunnerView(Config::GameContext gameCtx,
                          QWidget *parent = nullptr);
  ~GameRunnerView();
  void startLevel(const Levels::Level &level, bool benchmarkMode = false);
  void quitLevel();
  void resumeGame();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override { event->ignore(); }

private:
  Config::GameContext m_gameCtx;
  QOpenGLShaderProgram *m_program = nullptr;
  QOpenGLShaderProgram *m_lineProgram = nullptr;
  std::unique_ptr<UI::Panel> m_uiPanel;
  std::unique_ptr<UI::GLProgressBar> m_healthBar;
  std::unique_ptr<UI::GLProgressBar> m_energyBar;
  GameState *m_gameState;
  std::unique_ptr<Levels::LevelManager> m_levelManager;
  QTimer m_gameTimer;
  QTimer m_benchmarkTimer;
  std::shared_ptr<GameObjects::Ships::PlayerShip> m_playerShip;
  std::unique_ptr<CollisionDetection::CollisionDetector> m_collisionDetector;
  QElapsedTimer m_elapsedTimer;
  QElapsedTimer m_fpsTimer;
  QSet<int> m_pressedKeys;
  int m_frameCount = 0;
  QGraphicsTextItem *m_stellarTokens;
  QGraphicsTextItem *m_playerHp;
  QGraphicsTextItem *m_levelEndedInfo;
  bool m_continuousShoot;
  bool m_progressLevel;
  bool m_levelFailed;
  bool m_levelFailedOrPassedInfoDisplayed;
  bool m_spawnEventsFinished;
  bool m_benchmarkMode;
  std::chrono::high_resolution_clock::time_point m_lastFrameEndTime;

  GLuint m_texture = 0;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLVertexArrayObject m_debugVao;
  QOpenGLBuffer m_debugVbo;
  QOpenGLBuffer m_vbo;

  UI::FPSCounter *m_fpsCounter;
  UI::GameObjectCounter *m_gameObjectCounter;

  void setupView();
  void setupCounters();
  void setupConnections();
  inline void gameLoop();

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
         m_progressLevel = !m_progressLevel;
         m_pressedKeys.remove(Qt::Key_S);
       }},
  };

  inline float calculateRenderTime(
      const std::chrono::high_resolution_clock::time_point &loopStartTime);
  inline float calculateDeltaTime();
  inline void manageLevelProgression();
  template <typename Func> inline float measureFunctionDuration(Func &&func);
  inline void logFrameStatistics(float renderTimeUs, float updateTimeUs,
                                 float collisionDetectionTimeUs);
  inline void updateGameCounters();
  inline void checkLevelFailedOrPassed();
  void initializeBenchmark();
  void deinitializeBenchmark();
  void capFrameRate(float desiredMinFrameTimeSeconds, float frameTimeSeconds);
  void renderAllDebugColliders();
  void drawColliderBox(const GameObjects::GameObject *obj);
  void renderSprite(const GameObjects::GameObject *obj);
  void renderAllSprites();
signals:
  void fpsUpdated(int fps);
  void gamePaused();
  void levelQuit();
  void benchmarkFinished();

private slots:
  void onPlayerShipDestroyed() {
    m_levelFailed = true;
    m_playerShip = nullptr;
  }
  void onEnemyLimitReached() {
    m_levelFailed = true;
    m_playerShip = nullptr;
  }
  void onPlayerHealthUpdated(float value) {
      m_healthBar->setValue(value);
  }
  void onPlayerMaxHealthSet(float value) {
      m_healthBar->setRange(0, value);
  }
  void onPlayerEnergyUpdated(float value) {
      m_energyBar->setValue(value);
  }
  void onPlayerMaxEnergySet(float value) {
      m_energyBar->setRange(0, value);
  }
  void onSpawnEventsFinished() { m_spawnEventsFinished = true; }
  void pause() {
    m_gameTimer.stop();
    emit gamePaused();
  }
  void onBenchmarkFinished() { deinitializeBenchmark(); }
};
} // namespace Core
} // namespace Game

#endif // GAMERUNNERVIEW_H
