#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class LaserBeam : public Projectile {
public:
    LaserBeam(bool hostile = false, int damage = 1, std::set<Weapons::ProjectileProperty> properties = {});
  std::unique_ptr<Projectile> clone() const override;
protected:
  QPixmap getPixmap() const override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // LASERBEAM_H
