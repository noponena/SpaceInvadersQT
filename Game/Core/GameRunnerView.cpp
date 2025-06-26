#include "Game/Core/GameRunnerView.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Graphics/TextureRegistry.h"
#include "Utils/PerformanceBenchmark.h"
#include <QOpenGLWidget>
#include <QTimer>
#include <chrono>
#include <thread>

namespace Game {
namespace Core {

const char *vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 vert;
layout(location = 1) in vec2 uv;

uniform vec2 spritePos;
uniform vec2 spriteSize;
uniform float spriteRotation;
uniform vec2 viewport;

out vec2 fragUV;

void main() {
    // centered quad [-0.5, 0.5]
    vec2 centered = (vert - vec2(0.5, 0.5)) * spriteSize;
    float c = cos(spriteRotation);
    float s = sin(spriteRotation);
    mat2 rot = mat2(c, -s, s, c);
    vec2 rotated = rot * centered;

    vec2 pos = spritePos + rotated;
    vec2 ndc = (pos / viewport) * 2.0 - 1.0;
    ndc.y = -ndc.y; // invert Y for Qt
    gl_Position = vec4(ndc, 0, 1);

    fragUV = uv;
}
)";

const char *fragmentShaderSrc = R"(
#version 330 core
in vec2 fragUV;
out vec4 color;
uniform sampler2D tex;
void main() {
    color = texture(tex, fragUV);
}
)";

static const float quad[] = {
    // vert.x, vert.y, uv.x, uv.y
    0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
};

constexpr float MAX_FRAME_TIME =
    0.100f; // 100 ms = 10 FPS min (for game physics clamp)
constexpr float MIN_FRAME_TIME =
    0.005f; // 5 ms   = 200 FPS max (FPS cap for rendering/physics)

GameRunnerView::GameRunnerView(QRect screenGeometry, QWidget *parent)
    : m_continuousShoot(false), m_progressLevel(true), m_levelFailed(false),
      m_levelFailedOrPassedInfoDisplayed(false), m_spawnEventsFinished(false),
      m_benchmarkMode(false), m_vbo(QOpenGLBuffer::VertexBuffer) {
  m_gameState = new GameState();
  m_levelManager = std::make_unique<Levels::LevelManager>(m_gameState);
  m_gameObjects = &(m_gameState->gameObjects());
  m_collisionDetector = std::make_unique<CollisionDetection::CollisionDetector>(
      m_gameState->gameObjects(), screenGeometry);
  setupView();
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

  /*
  scene()->addItem(m_fpsCounter);
  scene()->addItem(m_gameObjectCounter);
  scene()->addItem(m_stellarTokens);
  scene()->addItem(m_playerHp);
*/
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

void GameRunnerView::startLevel(const Levels::Level &level,
                                bool benchmarkMode) {
  qDebug() << "starting game..";

  m_gameState->createPlayerShip();
  m_playerShip = m_gameState->playerShip();

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerSecondaryWeaponsChanged,
          m_gameHUD, &Core::GameHUD::onPlayerSecondaryWeaponsChanged);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerSecondaryWeaponFired,
          m_gameHUD, &Core::GameHUD::onPlayerSecondaryWeaponFired);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerEnergyUpdated, m_gameHUD,
          &Core::GameHUD::onPlayerEnergyUpdated);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerMaxEnergySet, m_gameHUD,
          &Core::GameHUD::onPlayerMaxEnergySet);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerHealthUpdated, m_gameHUD,
          &Core::GameHUD::onPlayerHealthUpdated);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerMaxHealthSet, m_gameHUD,
          &Core::GameHUD::onPlayerMaxHealthSet);

  m_gameState->initialize();

  if (benchmarkMode) {
    initializeBenchmark();
  }

  m_levelManager->setLevel(level);
  m_levelManager->startLevel();
  m_gameTimer.start(0);
}

void GameRunnerView::quitLevel() {
  m_gameTimer.stop();
  m_levelFailed = false;
  m_spawnEventsFinished = false;
  if (m_levelFailedOrPassedInfoDisplayed) {
    // scene()->removeItem(m_levelEndedInfo);
  }
  m_levelFailedOrPassedInfoDisplayed = false;
  m_gameState->deinitialize();
  m_playerShip = nullptr;
}

void GameRunnerView::resumeGame() {
  qDebug() << "resuming game..";
  m_elapsedTimer.start();
  m_gameTimer.start(0);
}

void GameRunnerView::initializeGL() {
  initializeOpenGLFunctions();

  // Compile shaders
  m_program = new QOpenGLShaderProgram();
  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     fragmentShaderSrc);
  m_program->link();

  // VAO/VBO for quad
  m_vao.create();
  m_vao.bind();
  m_vbo.create();
  m_vbo.bind();
  m_vbo.allocate(quad, sizeof(quad));

  m_program->enableAttributeArray(0);
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
  m_program->enableAttributeArray(1);
  m_program->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2,
                                4 * sizeof(float));

  m_vbo.release();
  m_vao.release();

  QImage img(32, 32, QImage::Format_RGBA8888);
  img.fill(Qt::white);
  for (int x = 0; x < 32; ++x)
    for (int y = 0; y < 32; ++y)
      if (x == 0 || y == 0 || x == 31 || y == 31)
        img.setPixelColor(x, y, Qt::red);

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               img.bits());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Graphics::TextureRegistry::instance().setGlContext(this);
  Graphics::TextureRegistry::instance().preloadAllFromDir(":/Images/");
}

void GameRunnerView::resizeGL(int w, int h) { QOpenGLWidget::resizeGL(w, h); }

void GameRunnerView::paintGL() {
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  m_program->bind();
  m_vao.bind();
  glActiveTexture(GL_TEXTURE0);
  m_program->setUniformValue("tex", 0);
  m_program->setUniformValue("viewport", QVector2D(width(), height()));

  qDebug() << "Rendering objects:" << m_gameObjects->size();

  for (const auto &obj : *m_gameObjects) {
    // Get position, bounding box, scale
    const auto &pos = obj->getPosition();

    // Use PixmapData for texture and custom scale
    GameObjects::PixmapData pixmapData = obj->getPixmapData();
    QString texPath = pixmapData.pixmapResourcePath;
    const auto &texInfo =
        Graphics::TextureRegistry::instance().getOrCreateTexture(texPath);
    GLuint texture = texInfo.handle;

    // Use either bounding box size, or custom scale from PixmapData
    QPointF size = pixmapData.pixmapScale;
    if (pixmapData.keepAspectRatio && texInfo.height > 0) {
      float aspect = float(texInfo.width) / texInfo.height;
      size.setX(size.y() * aspect); // or adjust as fits your logic
    }

    float rotation = 0.0f; // Or obj->getRotation() if available

    glBindTexture(GL_TEXTURE_2D, texture);

    m_program->setUniformValue("spritePos", QVector2D(pos.x(), pos.y()));
    m_program->setUniformValue("spriteSize", size);
    m_program->setUniformValue("spriteRotation", rotation);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }

  m_vao.release();
  m_program->release();
}

void GameRunnerView::gameLoop() {
  // auto loopStartTime = std::chrono::high_resolution_clock::now();
  // float renderTimeUs = calculateRenderTime(loopStartTime);
  float deltaTimeInSeconds = calculateDeltaTime();
  capFrameRate(MIN_FRAME_TIME, deltaTimeInSeconds);

  if (deltaTimeInSeconds > MAX_FRAME_TIME)
    deltaTimeInSeconds = MAX_FRAME_TIME;

  manageLevelProgression();

  if (!m_benchmarkMode)
    processInput(deltaTimeInSeconds);

  /*
  float updateTimeUs =
      measureFunctionDuration([&]() { updateGameState(deltaTimeInSeconds); });
  float collisionDetectionTimeUs =
      measureFunctionDuration([&]() { m_collisionDetector->detectBVH(); });

  logFrameStatistics(renderTimeUs, updateTimeUs, collisionDetectionTimeUs);
  */

  updateGameState(deltaTimeInSeconds);
  // m_collisionDetector->detectBVH();
  m_collisionDetector->detectBVHParallel();
  updateFps();

  updateGameCounters();
  checkLevelFailedOrPassed();
  m_lastFrameEndTime = std::chrono::high_resolution_clock::now();
  this->update();
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
  if (m_benchmarkMode) {
    Utils::PerformanceBenchmark::getInstance().recordFrameTime(frameTimeMs);
  }
  return static_cast<float>(frameTimeMs) / 1000.0f;
}

void GameRunnerView::capFrameRate(float desiredMinFrameTimeSeconds,
                                  float frameTimeSeconds) {
  int msToSleep = static_cast<int>(
      (desiredMinFrameTimeSeconds - frameTimeSeconds) * 1000.0f);
  if (msToSleep > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(msToSleep));
  }
}

void GameRunnerView::manageLevelProgression() {
  if (m_progressLevel) {
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
  connect(&m_benchmarkTimer, &QTimer::timeout, this, ([this]() {
    Utils::PerformanceBenchmark::getInstance().logPerformanceScore();
    onBenchmarkFinished();
  }));
  m_benchmarkMode = true;
  m_benchmarkTimer.start(30000);
}

void GameRunnerView::deinitializeBenchmark() {
  m_benchmarkTimer.disconnect();
  m_benchmarkTimer.stop();
  m_gameTimer.stop();
  m_benchmarkMode = false;
  emit benchmarkFinished();
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
  /*
  QRectF sceneRect = scene()->sceneRect();
  QPointF centerPosition =
      QPointF((sceneRect.width() - textBoundingRect.width()) / 2.0,
              (sceneRect.height() - textBoundingRect.height()) / 2.0);
  m_levelEndedInfo->setPos(centerPosition);
  m_levelFailedOrPassedInfoDisplayed = true;
  scene()->addItem(m_levelEndedInfo);
 */
}

void GameRunnerView::displayLevelPassedInfo() {
  m_levelEndedInfo->setPlainText("LEVEL SUCCESSFUL");
  QRectF textBoundingRect = m_levelEndedInfo->boundingRect();
  /*
  QRectF sceneRect = scene()->sceneRect();
  QPointF centerPosition =
      QPointF((sceneRect.width() - textBoundingRect.width()) / 2.0,
              (sceneRect.height() - textBoundingRect.height()) / 2.0);
  m_levelEndedInfo->setPos(centerPosition);
  m_levelFailedOrPassedInfoDisplayed = true;
  scene()->addItem(m_levelEndedInfo);
 */
}

void GameRunnerView::keyPressEvent(QKeyEvent *event) {
  m_pressedKeys.insert(event->key());
}

void GameRunnerView::keyReleaseEvent(QKeyEvent *event) {
  m_pressedKeys.remove(event->key());
}

} // namespace Core
} // namespace Game
// namespace Game
