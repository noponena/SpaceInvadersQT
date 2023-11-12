#ifndef WEAPONS_LASERCANNON_H
#define WEAPONS_LASERCANNON_H

#include "Weapon.h"
#include "GameObjects/Ships/Ship.h"

namespace Weapons {

template <typename ProjectileType>
class PrimaryWeapon : public Weapon {
public:
    PrimaryWeapon(float cooldownMs,
              Game::Movement::MovementStrategy movementStrategy, bool hostile = false, int damage = 1)
        : Weapon(cooldownMs, 100, movementStrategy, hostile, damage) {};

  // Weapon interface
public:
  GameObjects::Projectiles::Projectile *createProjectile() override
    {
        GameObjects::Position position = m_owner->getPosition();
        auto projectile = new ProjectileType(
            position, 1, m_hostile, m_damage, m_properties);
        projectile->setSoundEnabled(m_soundEnabled);
        return projectile;
    }
};

} // namespace Weapons

#endif // WEAPONS_LASERCANNON_H
