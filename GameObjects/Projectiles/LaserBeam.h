#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "Projectile.h"

namespace GameObjects
{
namespace Projectiles {
class LaserBeam : public Projectile, public QGraphicsRectItem
{
public:
    LaserBeam(Position position, float speed, Qt::GlobalColor color);
private:
    Qt::GlobalColor m_color;

    // GameObject interface
protected:
    void initializeGraphicsItem() override;
};
}

}



#endif // LASERBEAM_H
