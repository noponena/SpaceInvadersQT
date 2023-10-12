#include "Laser.h"
#include <QPen>

namespace GameObjects
{
Laser::Laser(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed), m_color(color)
{

}

void Laser::initialize()
{
    this->setMovementStrategy(Game::MovementStrategies::PlayerProjectileMovementStrategy(0.25, -1));
    QGraphicsRectItem* rectItem = new QGraphicsRectItem();
    rectItem->setRect(0, 0, 2, 10);
    rectItem->setBrush(m_color);
    rectItem->setPen(Qt::NoPen);
    m_graphicsItem = rectItem;
    this->updateGraphicsItemPosition();
}
}

