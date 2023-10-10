#include "Laser.h"
#include "Game/LinearMovement.h"
#include <QPen>

namespace GameObjects
{
Laser::Laser(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed)
{
    QGraphicsRectItem* rectItem = new QGraphicsRectItem();
    rectItem->setRect(0, 0, 2, 10);  // Adjust dimensions as needed
    rectItem->setBrush(color);  // Or any color you prefer
    m_graphicsItem = rectItem;
    this->updateBoundingBox();
}

void Laser::initialize()
{
    this->setXMovementFunc(Game::LinearMovement::get(0.5, -1));
    this->setYMovementFunc(Game::LinearMovement::get(0.5, -1));
}
}

