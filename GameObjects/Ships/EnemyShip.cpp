#include "EnemyShip.h"
#include "GameObjects/Effects/ParticleSystem.h"
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include <QTimerEvent>
#include <QColor>

namespace GameObjects {

EnemyShip::EnemyShip(const int maxHp, int speed, int fireRate, const Position &position)
    : Ship(maxHp, speed, fireRate, position)
{

}

void EnemyShip::initialize()
{
    //this->setMovementStrategy(Game::Movement::DownwardMovingCircularMovementStrategy(50, 1, m_speed));
    //this->setMovementStrategy(Game::Movement::CircularMovementStrategy(50, 1));
    this->initializeDestructionAnimation();
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();

    // Set the color and pen properties
    polygonItem->setBrush(Qt::green);
    polygonItem->setPen(Qt::NoPen);

    // Create a QPolygon to represent the triangle and set it to the QGraphicsPolygonItem
    QPolygon triangle;
    triangle << QPoint(0, 10) << QPoint(-10, -20) << QPoint(10, -20);  // Assuming the ship is centered at (0, 0)
    polygonItem->setPolygon(triangle);

    // Assign the polygonItem to m_graphicsItem
    m_graphicsItem = polygonItem;
    m_originalColor = static_cast<QGraphicsPolygonItem*>(m_graphicsItem)->brush().color();
    this->updateGraphicsItemPosition();
}

void EnemyShip::shoot()
{

}

void EnemyShip::collideWith(GameObject &other) {
    other.collideWithEnemyShip(*this);
}

void EnemyShip::collideWithProjectile(Projectile &projectile)
{
    int damageValue = projectile.getDamage();
    this->takeDamage(damageValue);
    if (this->isAlive())
        this->playOnHitAnimation();
}

void EnemyShip::collideWithEnemyShip(EnemyShip &enemyShip)
{
    this->takeDamage(0);
}

void EnemyShip::playOnDestructionAnimation() {
    if (m_onHitTimerId != -1) {
        killTimer(m_onHitTimerId);
        m_onHitTimerId = -1;
    }

    this->switchToPixmapItem();
    QPointF p(m_position.x(), m_position.y());
    Effects::ParticleSystem *particleSystem = new Effects::ParticleSystem(p);
    connect(particleSystem, &Effects::ParticleSystem::animationFinished, particleSystem, &QObject::deleteLater);
    particleSystem->spawnParticles(50);
    m_graphicsItem->scene()->addItem(particleSystem);
    particleSystem->start();
    m_frameIndex = 0;
    QTimer *animationTimer = new QTimer();
    connect(animationTimer, &QTimer::timeout, this, [this, animationTimer]() {
        QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(m_graphicsItem);
        if(pixmapItem && m_frameIndex < m_animationFrames.size()) {
            pixmapItem->setPixmap(m_animationFrames[m_frameIndex]);
            m_frameIndex++;
        } else {
            animationTimer->stop();
            animationTimer->deleteLater();
            emit animationCompleted();
        }
    });
    animationTimer->start(50);
}

void EnemyShip::playOnHitAnimation()
{
    if (m_onHitAnimationInProgress)
        return;

    m_onHitAnimationInProgress = true;
    QColor darkerColor = m_originalColor.darker(200);
    static_cast<QGraphicsPolygonItem*>(m_graphicsItem)->setBrush(darkerColor);

    m_onHitTimerId = startTimer(25);
}

void EnemyShip::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_onHitTimerId) {
        static_cast<QGraphicsPolygonItem*>(m_graphicsItem)->setBrush(m_originalColor);
        m_onHitAnimationInProgress = false;
        killTimer(m_onHitTimerId);
        m_onHitTimerId = -1;
    }
}

void EnemyShip::switchToPixmapItem() {
    if (m_graphicsItem) {
        QRectF rect = m_graphicsItem->boundingRect();

        qreal halfWidth = rect.width() / 2;
        qreal halfHeight = rect.height() / 2;

        this->moveX(-halfWidth);
        this->moveY(-halfHeight);

        // Create a new QGraphicsPixmapItem
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();

        // Replace the old m_graphicsItem with the new pixmap item
        QGraphicsScene* scene = m_graphicsItem->scene();
        scene->removeItem(m_graphicsItem);
        delete m_graphicsItem;
        m_graphicsItem = pixmapItem;
        scene->addItem(m_graphicsItem);
        QPointF p = pixmapItem->boundingRect().center();
        pixmapItem->setTransformOriginPoint(p);
    }
}

bool EnemyShip::shouldBeDeleted()
{
    return m_destroyed || m_position.isBeyondScreenBottomLimit(30);
    return false;
}
}
