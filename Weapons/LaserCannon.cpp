#include "LaserCannon.h"
#include "GameObjects/Projectiles/LaserBeam.h"
#include "GameObjects/Ships/Ship.h"

namespace Weapons {

LaserCannon::LaserCannon(float cooldownMs,
                         Game::Movement::MovementStrategy movementStrategy)
    : Weapon(cooldownMs, 200, movementStrategy)
{}

GameObjects::Projectiles::Projectile*
LaserCannon::createProjectile() {
  GameObjects::Position position = m_owner->getPosition();
  GameObjects::Projectiles::LaserBeam *laser =
      new GameObjects::Projectiles::LaserBeam(
          position, 1, Qt::GlobalColor::magenta);

  return laser;
}

} // namespace Weapons
