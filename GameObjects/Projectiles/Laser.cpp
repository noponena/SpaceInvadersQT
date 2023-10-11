#include "Laser.h"
#include <QPen>

namespace GameObjects
{
Laser::Laser(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed)
{
    QGraphicsRectItem* rectItem = new QGraphicsRectItem();
    rectItem->setRect(0, 0, 2, 10);
    rectItem->setBrush(color);
    rectItem->setPen(Qt::NoPen);
    m_graphicsItem = rectItem;
    this->initBoundingBox();
}

void Laser::initialize()
{
    this->setMovementStrategy(Game::MovementStrategies::PlayerProjectileMovementStrategy(0.5, -1));
}
}

