#include "PlayerShip.h"
#include "Weapons/LaserCannon.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const int maxHp, float speed, const Position &position)
    : Ship(maxHp, speed, position)
{
    m_lastShotTime.start();
}

void PlayerShip::collideWith(GameObject &other)
{
    other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectiles::Projectile &projectile)
{
    Q_UNUSED(projectile)
}

void PlayerShip::initializeGraphicsItem()
{
    // Create a new QGraphicsPolygonItem
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();

    // Set the color and pen properties
    polygonItem->setBrush(Qt::cyan);
    polygonItem->setPen(Qt::NoPen);
    polygonItem->setPen(QPen(Qt::black));

    // Create a QPolygon to represent the triangle and set it to the QGraphicsPolygonItem
    QPolygon triangle;
    triangle << QPoint(0, -10) << QPoint(-5, 10) << QPoint(5, 10);
    polygonItem->setPolygon(triangle);

    // Assign the polygonItem to m_graphicsItem
    m_graphicsItem = polygonItem;
    this->setWeapon(std::make_unique<Weapons::LaserCannon>(1000, Game::Movement::VerticalMovementStrategy(500, -1)));
    this->updateGraphicsItemPosition();
}
}

}


