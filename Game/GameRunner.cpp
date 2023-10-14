#include "GameRunner.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "UI/FPSCounter.h"
#include "UI/GameObjectCounter.h"
#include <QOpenGLWidget>
#include <QTimer>

namespace Game {
GameRunner::GameRunner(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this))
{
    setupView();
    setupCounters();
    setupConnections();
    m_elapsedTimer.start();
}

void GameRunner::setupView()
{
    setViewport(new QOpenGLWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setInteractive(false);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::DontSavePainterState);
    setViewportMargins(0, 0, 0, 0);
    setScene(&m_scene);
    m_scene.setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    m_scene.setBackgroundBrush(QBrush(Qt::black));
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GameRunner::setupCounters()
{
    UI::FPSCounter *fpsCounter = new UI::FPSCounter();
    UI::GameObjectCounter *gameObjectCounter = new UI::GameObjectCounter();
    fpsCounter->setPos(0, 0);
    gameObjectCounter->setPos(0, fpsCounter->boundingRect().height() - 10);
    scene()->addItem(fpsCounter);
    scene()->addItem(gameObjectCounter);

    connect(&m_gameState, &GameState::objectAdded, this, [=](){
        gameObjectCounter->updateObjectCount(1);
    });
    connect(&m_gameState, &GameState::objectDeleted, this, [=](){
        gameObjectCounter->updateObjectCount(-1);
    });
    connect(this, &GameRunner::fpsUpdated, fpsCounter, &UI::FPSCounter::updateFPS);
}

void GameRunner::setupConnections()
{
    connect(&m_gameState, &GameState::objectAdded, this, &GameRunner::onObjectAdded);
    connect(&m_gameState, &GameState::objectDeleted, this, &GameRunner::onObjectDeleted);
}

void GameRunner::startGame()
{
    // Initialize game state
    qDebug() << "starting game..";
    m_gameState.setSize(this->scene()->sceneRect().width(), this->scene()->sceneRect().height());
    m_gameState.initialize();
    m_playerShip = m_gameState.playerShip();
    m_gameObjects = &(m_gameState.gameObjects());
    m_levelManager = std::make_unique<LevelManager>(m_gameState);

    // Create and start game loop timer
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameRunner::gameLoop);
    timer->start(16);  // Approx. 60 frames per second

    // Hide menu and show game window
    // ...
}

void GameRunner::gameLoop()
{
    float deltaTimeInSeconds = static_cast<float>(m_elapsedTimer.restart()) / 1000.0f;
    m_levelManager->update();
    this->processInput(deltaTimeInSeconds);
    this->updateGameState(deltaTimeInSeconds);
    this->detectCollisions();
    this->updateFps();
}

void GameRunner::processInput(float deltaTime)
{
    for (const auto& [key, action] : m_keyActions) {
        if (m_pressedKeys.contains(key))
            action(deltaTime);
    }

    if ((!m_pressedKeys.contains(Qt::Key_Left) && !m_pressedKeys.contains(Qt::Key_Right)) ||
        (m_pressedKeys.contains(Qt::Key_Left) && m_pressedKeys.contains(Qt::Key_Right))) {
        m_playerShip->decelerateX(deltaTime);
    }

    if ((!m_pressedKeys.contains(Qt::Key_Up) && !m_pressedKeys.contains(Qt::Key_Down)) ||
        (m_pressedKeys.contains(Qt::Key_Up) && m_pressedKeys.contains(Qt::Key_Down))) {
        m_playerShip->decelerateY(deltaTime);
    }

    m_playerShip->moveHorizontal(deltaTime);
    m_playerShip->moveVertical(deltaTime);
}

void GameRunner::updateGameState(float deltaTime)
{
    m_gameState.update(deltaTime);
}

void GameRunner::updateFps()
{
    m_frameCount++;
    if (m_fpsTimer.elapsed() >= 1000) {
        emit fpsUpdated(m_frameCount);
        m_frameCount = 0;
        m_fpsTimer.restart();
    }
}

void GameRunner::detectCollisions()
{
    for (auto it1 = m_gameObjects->begin(); it1 != m_gameObjects->end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != m_gameObjects->end(); ++it2) {
            if ((*it1)->isCollision(**it2)) {
                (*it1)->doCollide(**it2);
            }
        }
    }
}

void GameRunner::keyPressEvent(QKeyEvent *event)
{
    m_pressedKeys.insert(event->key());
}

void GameRunner::keyReleaseEvent(QKeyEvent *event)
{
    m_pressedKeys.remove(event->key());
}

}
