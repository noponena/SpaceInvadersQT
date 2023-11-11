#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class LaserBeam : public Projectile {
public:
  LaserBeam(Position position, float speed, bool hostile = false, int damage = 1, std::set<Weapons::WeaponProperty> properties = {});

  // GameObject interface
protected:
  QPixmap getPixmap() const override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // LASERBEAM_H
