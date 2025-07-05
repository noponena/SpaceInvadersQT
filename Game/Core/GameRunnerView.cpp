#include "Game/Core/GameRunnerView.h"
#include "Game/Audio/SoundManager.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include "Graphics/Effects/EffectManager.h"
#include "Graphics/TextureRegistry.h"
#include "Utils/PerformanceBenchmark.h"
#include <QOpenGLContext>
#include <QTimer>
#include <QVector2D>
#include <chrono>
#include <thread>

namespace Game {
namespace Core {

const char *line_vertex = R"(
#version 330 core
layout(location = 0) in vec2 position;

uniform vec2 viewport;

void main() {
    vec2 ndc = (position / viewport) * 2.0 - 1.0;
    ndc.y = -ndc.y; // OpenGL/Qt Y flip
    gl_Position = vec4(ndc, 0.0, 1.0);
}
)";

const char *line_fragment = R"(
#version 330 core
out vec4 fragColor;
uniform vec3 lineColor;
void main() {
    fragColor = vec4(lineColor, 1.0);
}

)";

const char *vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 vert;
layout(location = 1) in vec2 uv;

uniform vec2 spritePos;
uniform vec2 spriteSize;
uniform float spriteRotation;
uniform vec2 viewport;
uniform vec2 uvMin;
uniform vec2 uvMax;

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

    fragUV = mix(uvMin, uvMax, uv);
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
constexpr bool RENDER_COLLIDER_BOXES = false;

GameRunnerView::GameRunnerView(Config::GameContext ctx, QWidget *parent)
    : m_gameCtx(ctx), m_continuousShoot(false), m_progressLevel(true),
      m_levelFailed(false), m_levelFailedOrPassedInfoDisplayed(false),
      m_spawnEventsFinished(false), m_benchmarkMode(false),
      m_debugVbo(QOpenGLBuffer::VertexBuffer),
      m_vbo(QOpenGLBuffer::VertexBuffer) {
  m_gameState = new GameState(ctx);
  m_levelManager = std::make_unique<Levels::LevelManager>(m_gameState);
  m_gameObjects = &(m_gameState->gameObjects());
  m_collisionDetector = std::make_unique<CollisionDetection::CollisionDetector>(
      m_gameState->gameObjects(), ctx.screenGeometry);
  setupView();
  setupCounters();
  m_elapsedTimer.start();
  m_fpsTimer.start();

  setupConnections();
}

GameRunnerView::~GameRunnerView() {
  // We have to make sure that the game objects
  // are destroyed before the scene (QGraphicsScene)
  // destroys the graphics items in the scene.
  delete m_gameState;
  delete m_program;
  delete m_lineProgram;
  delete m_fpsCounter;
  delete m_gameObjectCounter;
  delete m_stellarTokens;
  delete m_playerHp;
  delete m_levelEndedInfo;

  m_vao.destroy();
  m_debugVao.destroy();
  m_vbo.destroy();
  if (m_texture != 0)
    glDeleteTextures(1, &m_texture);
}

void GameRunnerView::setupView() {
  QSurfaceFormat format;
  format.setSwapInterval(0);
  format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
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
          &GameObjects::Ships::PlayerShip::playerHealthUpdated,
          this,
          &GameRunnerView::onPlayerHealthUpdated);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerMaxHealthSet, this,
          &GameRunnerView::onPlayerMaxHealthSet);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerEnergyUpdated,
          this,
          &GameRunnerView::onPlayerEnergyUpdated);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerMaxEnergySet, this,
          &GameRunnerView::onPlayerMaxEnergySet);

  /*
  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerSecondaryWeaponsChanged,
          m_gameHUD, &GameHUD::onPlayerSecondaryWeaponsChanged);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerSecondaryWeaponFired,
          m_gameHUD, &GameHUD::onPlayerSecondaryWeaponFired);


  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerEnergyUpdated, m_gameHUD,
          &GameHUD::onPlayerEnergyUpdated);

  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerMaxEnergySet, m_gameHUD,
          &GameHUD::onPlayerMaxEnergySet);
*/

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
  Graphics::Effects::EffectManager::instance().clear();
}

void GameRunnerView::resumeGame() {
  qDebug() << "resuming game..";
  m_elapsedTimer.start();
  m_gameTimer.start(0);
}

void GameRunnerView::initializeGL() {

  initializeOpenGLFunctions();
  Graphics::Effects::EffectManager::instance().initializeGL(this);

  m_healthBar = std::make_unique<UI::GLProgressBar>(
      0.f, 100.f,      // Min/max
      0.7f, 0.015f,     // 30% width, 6% height (fractions)
      UI::UISizeMode::Fraction
      );
  m_healthBar->setBarColors(
      QVector4D(0.1f, 0.7f, 0.2f, 1.f),   // Green
      QVector4D(0.95f, 0.83f, 0.29f, 1.f),// Yellow
      QVector4D(0.93f, 0.24f, 0.24f, 1.f) // Red
      );
  m_healthBar->setThresholds(0.6f, 0.3f); // 60%/30% thresholds
  m_healthBar->setCenter(0.5f, 0.952f);

  m_energyBar = std::make_unique<UI::GLProgressBar>(
      0.f, 100.f,      // Min/max
      0.7f, 0.015f,     // 30% width, 6% height (fractions)
      UI::UISizeMode::Fraction
      );
  m_energyBar->setBarColors(
      QVector4D(24.f/255.f, 40.f/255.f, 119.f/255.f, 1.f), // Blue
      QVector4D(24.f/255.f, 40.f/255.f, 119.f/255.f, 1.f),
      QVector4D(24.f/255.f, 40.f/255.f, 119.f/255.f, 1.f)
      );
  m_energyBar->setCenter(0.5f, 0.97f);

  m_uiPanel = std::make_unique<UI::Panel>();

  // === 1. Compile and link shaders ===
  // Main sprite shader
  m_program = new QOpenGLShaderProgram();
  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc);
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     fragmentShaderSrc);
  m_program->link();

  // Debug line shader
  m_lineProgram = new QOpenGLShaderProgram();
  m_lineProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, line_vertex);
  m_lineProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                         line_fragment);
  m_lineProgram->link();

  // === 2. Main sprite VAO/VBO setup ===
  m_vao.create();
  m_vao.bind();

  m_vbo.create();
  m_vbo.bind();
  m_vbo.allocate(quad, sizeof(quad)); // quad = your quad vertex data

  m_program->enableAttributeArray(0); // vert.xy
  m_program->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(float));
  m_program->enableAttributeArray(1); // uv.xy
  m_program->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(float), 2,
                                4 * sizeof(float));

  m_vbo.release();
  m_vao.release();

  // === 3. Debug line VAO/VBO setup ===
  m_debugVao.create();
  m_debugVao.bind();

  m_debugVbo.create();
  m_debugVbo.bind();
  m_debugVbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  m_debugVbo.allocate(sizeof(QVector2D) * 4);

  m_lineProgram->enableAttributeArray(0);
  m_lineProgram->setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);

  m_debugVbo.release();
  m_debugVao.release();

  // === 4. Texture setup (unchanged) ===
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

  m_uiPanel->render(this, m_gameCtx.screenGeometry.width(), m_gameCtx.screenGeometry.height());
  m_healthBar->render(this, width(), height());
  m_energyBar->render(this, width(), height());
  renderAllSprites();

  Graphics::Effects::EffectManager::instance().render(
      QVector2D(width(), height()));

#ifndef NDEBUG
  if (RENDER_COLLIDER_BOXES)
    renderAllDebugColliders();
#endif
}

// -- Main sprite rendering for all objects --
void GameRunnerView::renderAllSprites() {
  m_program->bind();
  m_vao.bind();
  glActiveTexture(GL_TEXTURE0);
  m_program->setUniformValue("tex", 0);
  m_program->setUniformValue("viewport", QVector2D(width(), height()));

  for (const auto &obj : *m_gameObjects) {
    if (!obj->isVisible())
      continue;
    renderSprite(obj.get());
  }

  glBindVertexArray(0);
  m_vao.release();
  m_program->release();
}

void GameRunnerView::renderSprite(const GameObjects::GameObject *obj) {
  const auto &pos = obj->getPosition();
  const GameObjects::RenderData renderData = obj->getRenderData();
  const auto &texInfo =
      Graphics::TextureRegistry::instance().getOrCreateTexture(
          renderData.imagePath);
  GLuint texture = texInfo.handle;

  glBindTexture(GL_TEXTURE_2D, texture);

  m_program->setUniformValue("uvMin", renderData.uvMin);
  m_program->setUniformValue("uvMax", renderData.uvMax);
  m_program->setUniformValue("spritePos", pos);
  m_program->setUniformValue("spriteSize", renderData.size);
  m_program->setUniformValue("spriteRotation", renderData.rotation);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

// -- Debug collider rendering for all objects --
void GameRunnerView::renderAllDebugColliders() {
  m_debugVao.bind();
  m_lineProgram->bind();
  m_lineProgram->setUniformValue("viewport", QVector2D(width(), height()));
  m_lineProgram->setUniformValue("lineColor", QVector3D(1, 0, 0)); // Red

  for (const auto &obj : *m_gameObjects) {
    if (!obj->isVisible())
      continue;
    drawColliderBox(obj.get());
  }

  glBindVertexArray(0);
  m_lineProgram->release();
  m_debugVao.release();
}

void GameRunnerView::drawColliderBox(const GameObjects::GameObject *obj) {
  QVector2D collider = obj->transform().colliderSize;
  QVector2D center = obj->getPosition();

  float x1 = center.x() - 0.5f * collider.x();
  float x2 = center.x() + 0.5f * collider.x();
  float y1 = center.y() - 0.5f * collider.y();
  float y2 = center.y() + 0.5f * collider.y();

  QVector2D colliderVerts[] = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};

  m_debugVbo.bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colliderVerts), colliderVerts);
  glDrawArrays(GL_LINE_LOOP, 0, 4);
  m_debugVbo.release();
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
  m_collisionDetector->detectBVHParallel();
  Graphics::Effects::EffectManager::instance().update(deltaTimeInSeconds);
  updateFps();

  updateGameCounters();
  checkLevelFailedOrPassed();
  Game::Audio::SoundManager::getInstance().cleanup();
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
        Utils::PerformanceBenchmark::getInstance(m_gameCtx).recordFrameTime(
            frameTimeMs);
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
  Utils::PerformanceBenchmark::getInstance(m_gameCtx).initializeBenchmark(
      m_playerShip);
  connect(&m_benchmarkTimer, &QTimer::timeout, this, ([this]() {
    Utils::PerformanceBenchmark::getInstance(m_gameCtx).logPerformanceScore();
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
