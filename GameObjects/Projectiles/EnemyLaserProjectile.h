#ifndef GAMEOBJECTS_PROJECTILES_ENEMYLASERBEAM_H
#define GAMEOBJECTS_PROJECTILES_ENEMYLASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {

class EnemyLaserProjectile : public Projectile {
public:
  EnemyLaserProjectile(
      int damage = 1,
      std::unordered_set<Weapons::ProjectileProperty> properties = {});

  // Projectile interface
public:
  std::unique_ptr<Projectile> clone() const override;

  // GameObject interface
public:
  void initializeObjectType() override;
  void initializeGraphics() override;
  void initializeSounds() override;
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_ENEMYLASERBEAM_H
