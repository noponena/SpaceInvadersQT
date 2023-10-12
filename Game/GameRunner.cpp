#include "GameRunner.h"
#include "GameObjects/Ships/PlayerShip.h"
#include <QGraphicsPolygonItem>
#include <QOpenGLWidget>
#include <QThread>
#include "UI/FPSCounter.h"
#include "CollisionDetector.h"

namespace Game {
GameRunner::GameRunner(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this))
{
    setViewport(new QOpenGLWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setInteractive(false);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | QGraphicsView::DontSavePainterState);
    setViewportMargins(0, 0, 0, 0);
    //setRenderHint(QPainter::Antialiasing);
    setScene(&m_scene);
    UI::FPSCounter *fpsCounter = new UI::FPSCounter();
    fpsCounter->setPos(0, 0);
    scene()->setItemIndexMethod(QGraphicsScene::BspTreeIndex);
    scene()->addItem(fpsCounter);
    m_scene.setBackgroundBrush(QBrush(Qt::black));
    m_elapsedTimer.start();
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(&m_gameState, &GameState::objectAdded, this, &GameRunner::onObjectAdded);
    connect(&m_gameState, &GameState::objectDeleted, this, &GameRunner::onObjectDeleted);
    connect(this, &GameRunner::fpsUpdated, fpsCounter, &UI::FPSCounter::updateFPS);
}

GameRunner::~GameRunner()
{
//    m_collisionThread->quit();
//    m_collisionThread->wait();
//    delete m_collisionThread;
}

void GameRunner::startGame()
{
    // Initialize game state
    m_gameState.setSize(this->scene()->sceneRect().width(), this->scene()->sceneRect().height());
    m_gameState.initialize();
    m_gameObjects = &(m_gameState.gameObjects());
    //this->initializeCollisionDetection();

    // Create and start game loop timer
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameRunner::gameLoop);
    timer->start(16);  // Approx. 60 frames per second

    // Hide menu and show game window
    // ...
}


void GameRunner::keyPressEvent(QKeyEvent *event)
{
    m_pressedKeys.insert(event->key());
}

void GameRunner::keyReleaseEvent(QKeyEvent *event)
{
    m_pressedKeys.remove(event->key());
}

QGraphicsScene *GameRunner::scene()
{
    return &m_scene;
}


void GameRunner::gameLoop()
{
    qint64 deltaTime = m_elapsedTimer.restart();
    this->processInput(deltaTime);
    this->updateGameState(deltaTime);
    this->detectCollisions();
    this->updateFps();
}

void GameRunner::processInput(qint64 deltaTime)
{
    std::shared_ptr<GameObjects::PlayerShip> playerShip = m_gameState.playerShip();
    if (m_pressedKeys.contains(Qt::Key_Left)) {
        playerShip->moveLeft(deltaTime);
    }
    if (m_pressedKeys.contains(Qt::Key_Right)) {
        playerShip->moveRight(deltaTime);
    }
    if (m_pressedKeys.contains(Qt::Key_Down)) {
        playerShip->moveDown(deltaTime);
    }
    if (m_pressedKeys.contains(Qt::Key_Up)) {
        playerShip->moveUp(deltaTime);
    }
    if (m_pressedKeys.contains(Qt::Key_Space) || m_perfTest) {
        playerShip->shoot();
    }
    if (m_pressedKeys.contains(Qt::Key_Q)) {
        m_gameState.initEnemyShips();
        m_pressedKeys.remove(Qt::Key_Q);
    }
    if (m_pressedKeys.contains(Qt::Key_U)) {
        playerShip->updateFireRate();
        int rate = playerShip->fireRate();
        qDebug() << "fireRate:" << rate;
    }
    if (m_pressedKeys.contains(Qt::Key_D)) {
        playerShip->updateFireRate(-1);
        int rate = playerShip->fireRate();
        qDebug() << "fireRate:" << rate;
    }
}

void GameRunner::updateGameState(qint64 deltaTime)
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

void GameRunner::initializeCollisionDetection()
{
    m_mutex = &m_gameState.mutex();
    m_collisionThread = new QThread;
    CollisionDetector* detector = new CollisionDetector(m_gameState.gameObjects(), m_gameState.mutex());
    detector->moveToThread(m_collisionThread);

    connect(m_collisionThread, &QThread::finished, detector, &CollisionDetector::deleteLater);
    connect(detector, &CollisionDetector::collisionDetected,
                     this, &GameRunner::onCollisionDetected,
                     Qt::QueuedConnection);

    m_collisionThread->start();
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
}


