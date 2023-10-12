#ifndef LASER_H
#define LASER_H

#include "Projectile.h"

namespace GameObjects
{
class Laser : public Projectile, public QGraphicsRectItem
{
public:
    Laser(Position position, float speed, Qt::GlobalColor color);
    void initialize() override;
private:
    Qt::GlobalColor m_color;
};
}



#endif // LASER_H
