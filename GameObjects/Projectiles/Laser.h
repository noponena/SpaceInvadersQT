#ifndef LASER_H
#define LASER_H

#include "Projectile.h"

namespace GameObjects
{
class Laser : public Projectile
{
public:
    Laser(Position position, float speed, Qt::GlobalColor color);
    void initialize() override;
};
}



#endif // LASER_H
