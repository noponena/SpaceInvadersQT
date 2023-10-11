#include "EnemyShip.h"
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>

namespace GameObjects {

EnemyShip::EnemyShip(const int maxHp, float speed, int fireRate, const Position &position)
    : Shooter(maxHp, speed, fireRate, position)
{}

void EnemyShip::initialize()
{
    this->setMovementStrategy(Game::MovementStrategies::CircularMovementStrategy(2, 1));
    //this->setMovementStrategy(Game::MovementStrategies::PlayerProjectileMovementStrategy(0.07));
    this->initializeDestructionAnimation();
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();

    // Set the color and pen properties
    polygonItem->setBrush(Qt::green);
    //polygonItem->setPen(QPen(Qt::black));
    polygonItem->setPen(Qt::NoPen);

    // Create a QPolygon to represent the triangle and set it to the QGraphicsPolygonItem
    QPolygon triangle;
    triangle << QPoint(0, 10) << QPoint(-10, -20) << QPoint(10, -20);  // Assuming the ship is centered at (0, 0)
    polygonItem->setPolygon(triangle);

    // Assign the polygonItem to m_graphicsItem
    m_graphicsItem = polygonItem;
    this->initBoundingBox();
}

void EnemyShip::shoot()
{

}

void EnemyShip::collideWith(GameObject &other) {
    other.collideWithEnemyShip(*this);
}

void EnemyShip::collideWithProjectile(Projectile &projectile)
{
    int damageValue = projectile.getDamage();  // Assume Projectile has a method damage()
    this->takeDamage(damageValue);
}

void EnemyShip::collideWithEnemyShip(EnemyShip &enemyShip)
{
    this->takeDamage(10000);
}

void EnemyShip::playDestructionAnimation() {
    this->switchToPixmapItem();  // Switch to using a QGraphicsPixmapItem
    this->moveX(-15);
    this->moveY(-20);
    m_frameIndex = 0;  // Reset frameIndex
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
    animationTimer->start(100);
}



void EnemyShip::switchToPixmapItem() {
    if (m_graphicsItem) {
        // Get the current position of the polygon item
        QPointF position(500, 500);// = m_graphicsItem->scenePos();

        // Create a new QGraphicsPixmapItem
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();

        // Set the position of the new pixmap item to match the old polygon item
        pixmapItem->setPos(position);

        // Replace the old m_graphicsItem with the new pixmap item
        QGraphicsScene* scene = m_graphicsItem->scene();
        scene->removeItem(m_graphicsItem);
        delete m_graphicsItem;
        m_graphicsItem = pixmapItem;
        scene->addItem(m_graphicsItem);
        pixmapItem->setZValue(1000);
        pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());
    }
}

bool EnemyShip::shouldBeDeleted()
{
    return m_destroyed || m_position.isBeyondScreenBottomLimit(30);
}

} // namespace GameObjects
