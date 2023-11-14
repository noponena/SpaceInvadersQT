#ifndef PLAYERLASERBEAM_H
#define PLAYERLASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class PlayerLaserBeam : public Projectile {
public:
  PlayerLaserBeam(
      int damage = 1,
      std::unordered_set<Weapons::ProjectileProperty> properties = {});
  std::unique_ptr<Projectile> clone() const override;

protected:
  QPixmap getPixmap() const override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // PLAYERLASERBEAM_H
