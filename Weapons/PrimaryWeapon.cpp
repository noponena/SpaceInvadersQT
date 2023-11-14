#include "PrimaryWeapon.h"
#include "GameObjects/Projectiles/LaserBeam.h"
#include "GameObjects/Ships/Ship.h"

namespace Weapons {

PrimaryWeapon::PrimaryWeapon(float cooldownMs,
                             Game::Movement::MovementStrategy movementStrategy,
                             bool hostile, int damage)
    : Weapon(cooldownMs, 200, movementStrategy, hostile, damage) {}

GameObjects::Projectiles::Projectile *PrimaryWeapon::createProjectile() {
  GameObjects::Position position = m_owner->getPosition();
  GameObjects::Projectiles::LaserBeam *laser =
      new GameObjects::Projectiles::LaserBeam(position, 1, m_hostile, m_damage,
                                              m_properties);

  laser->setSoundEnabled(m_soundEnabled);
  return laser;
}

} // namespace Weapons
