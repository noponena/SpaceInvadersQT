#include "LaserBeam.h"
#include <QPen>

namespace GameObjects
{
namespace Projectiles {
LaserBeam::LaserBeam(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed), m_color(color)
{

}

void LaserBeam::initialize()
{
    QGraphicsRectItem* rectItem = new QGraphicsRectItem();
    rectItem->setRect(0, 0, 2, 10);
    rectItem->setBrush(m_color);
    rectItem->setPen(Qt::NoPen);
    m_graphicsItem = rectItem;
    this->updateGraphicsItemPosition();
}
}

}

