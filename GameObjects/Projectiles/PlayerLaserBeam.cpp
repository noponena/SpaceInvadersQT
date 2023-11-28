#include "PlayerLaserBeam.h"
#include "Game/Audio/SoundInfo.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
PlayerLaserBeam::PlayerLaserBeam(
    int damage, std::unordered_set<Weapons::ProjectileProperty> properties)
    : Projectile(damage, properties) {}

std::unique_ptr<Projectile> PlayerLaserBeam::clone() const {
  std::unique_ptr<PlayerLaserBeam> laserBeam =
      std::make_unique<PlayerLaserBeam>();
  laserBeam->setDamage(m_damage);
  laserBeam->setProperties(m_properties);
  laserBeam->setMovementStrategy(movementStrategy());
  return laserBeam;
}

void PlayerLaserBeam::initializeObjectType() {
  m_objectType = ObjectType::PLAYER_PROJECTILE;
}

void PlayerLaserBeam::initializeGraphics() {
  m_pixmapResourcePath = ":/Images/player_laser_projectile.png";
  m_pixmapScale = QPointF(29.055, 30.0);
}

void PlayerLaserBeam::initializeSounds() {
  m_spawnSoundInfo =
      Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::LASER});
}

} // namespace Projectiles
} // namespace GameObjects
