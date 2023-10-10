#include "PlayerShip.h"
#include <QPen>

namespace GameObjects {

PlayerShip::PlayerShip(const int maxHp, float speed, int fireRate, const Position &position)
    : Shooter(maxHp, speed, fireRate, position)
{
    m_lastShotTime.start();
}

void PlayerShip::initialize()
{
    // Create a new QGraphicsPolygonItem
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();

    // Set the color and pen properties
    polygonItem->setBrush(Qt::cyan);
    polygonItem->setPen(QPen(Qt::black));

    // Create a QPolygon to represent the triangle and set it to the QGraphicsPolygonItem
    QPolygon triangle;
    triangle << QPoint(0, -10) << QPoint(-5, 10) << QPoint(5, 10);  // Assuming the ship is centered at (0, 0)
    polygonItem->setPolygon(triangle);

    // Assign the polygonItem to m_graphicsItem
    m_graphicsItem = polygonItem;
    this->updateBoundingBox();

//    m_animatedItem = new AnimatedGraphicsItem(m_graphicsItem.data());  // assuming m_graphicsItem is a QSharedPointer<QGraphicsPolygonItem>
//    m_animation = new QPropertyAnimation(m_animatedItem, "pos");
//    m_animation->setDuration(100);  // for a half-second animation
    //    m_animation->setEasingCurve(QEasingCurve::Linear);
}

void PlayerShip::shoot()
{
    if (this->canShoot())
    {
        Position position = m_position;
        position.moveY(-10);
        std::shared_ptr<Laser> laser = std::make_shared<Laser>(position, 1, Qt::red);
        laser->initialize();
        emit laserShot(laser);
    }
}

void PlayerShip::collideWith(GameObject &other)
{
    other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectile &projectile)
{
    if (!projectile.hostile())
    {
        qDebug() << "Friendly fire!";
    }
}

}


