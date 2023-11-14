#ifndef GAMEOBJECTS_PROJECTILES_ENEMYLASERBEAM_H
#define GAMEOBJECTS_PROJECTILES_ENEMYLASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {

class EnemyLaserBeam : public Projectile {
public:
  EnemyLaserBeam(
      int damage = 1,
      std::unordered_set<Weapons::ProjectileProperty> properties = {});

  // Projectile interface
public:
  std::unique_ptr<Projectile> clone() const override;

protected:
  QPixmap getPixmap() const override;
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_ENEMYLASERBEAM_H
