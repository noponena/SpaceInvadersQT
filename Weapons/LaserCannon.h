#ifndef WEAPONS_LASERCANNON_H
#define WEAPONS_LASERCANNON_H

#include "Weapon.h"

namespace Weapons {

class LaserCannon : public Weapon {
public:
  LaserCannon(float cooldownMs,
              Game::Movement::MovementStrategy movementStrategy);

  // Weapon interface
public:
  GameObjects::Projectiles::Projectile *createProjectile() override;
};

} // namespace Weapons

#endif // WEAPONS_LASERCANNON_H
