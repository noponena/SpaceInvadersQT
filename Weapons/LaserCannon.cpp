#include "LaserCannon.h"
#include "GameObjects/Projectiles/LaserBeam.h"
#include "GameObjects/Ships/Ship.h"

namespace Weapons {

LaserCannon::LaserCannon(float cooldownMs,
                         Game::Movement::MovementStrategy movementStrategy)
    : Weapon(cooldownMs, 100, movementStrategy)
{}

std::shared_ptr<GameObjects::Projectiles::Projectile>
LaserCannon::createProjectile() {
  GameObjects::Position position = m_owner->getPosition();
  std::shared_ptr<GameObjects::Projectiles::LaserBeam> laser =
      std::make_shared<GameObjects::Projectiles::LaserBeam>(
          position, 1, Qt::GlobalColor::magenta);

  return laser;
}

} // namespace Weapons
