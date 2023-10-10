#include "Laser.h"
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
    this->calculateBoundingBox();
}

void Laser::initialize()
{

}
}

