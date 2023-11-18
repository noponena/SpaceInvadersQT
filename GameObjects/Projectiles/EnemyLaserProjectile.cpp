#include "EnemyLaserProjectile.h"

namespace GameObjects {
namespace Projectiles {

EnemyLaserProjectile::EnemyLaserProjectile(
    int damage, std::unordered_set<Weapons::ProjectileProperty> properties)
    : Projectile(damage, properties) {}

std::unique_ptr<Projectile> EnemyLaserProjectile::clone() const {
  std::unique_ptr<EnemyLaserProjectile> laserBeam =
      std::make_unique<EnemyLaserProjectile>();
  laserBeam->setDamage(m_damage);
  laserBeam->setProperties(m_properties);
  laserBeam->setMovementStrategy(movementStrategy());
  return laserBeam;
}

void EnemyLaserProjectile::initializeObjectType() {
  m_objectType = ObjectType::ENEMY_PROJECTILE;
}

void Projectiles::EnemyLaserProjectile::initializeGraphics() {
  m_pixmapResourcePath = ":/Images/enemy_laser_projectile.png";
  m_pixmapScale = QPointF(29.055, 30.0);
}

void EnemyLaserProjectile::initializeSounds() {
  m_spawnSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_LASER});
}
} // namespace Projectiles
} // namespace GameObjects
