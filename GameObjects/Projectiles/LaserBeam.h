#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class LaserBeam : public Projectile {
public:
  LaserBeam(Position position, float speed, Qt::GlobalColor color);

  // GameObject interface
protected:
  QPixmap getPixmap() const override;

private:
  Qt::GlobalColor m_color;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // LASERBEAM_H
