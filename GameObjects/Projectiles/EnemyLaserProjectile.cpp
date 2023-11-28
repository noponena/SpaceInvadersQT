#include "EnemyLaserProjectile.h"

namespace GameObjects {
namespace Projectiles {

EnemyLaserProjectile::EnemyLaserProjectile(
    int damage, std::unordered_set<ProjectileProperty> properties)
    : Projectile(damage, properties) {}

std::unique_ptr<Projectile> EnemyLaserProjectile::clone() const {
  std::unique_ptr<EnemyLaserProjectile> laserBeam =
      std::make_unique<EnemyLaserProjectile>();
  laserBeam->setPixmapData(m_pixmapData);
  laserBeam->setDamage(m_damage);
  laserBeam->setProperties(m_properties);
  laserBeam->setMovementStrategy(movementStrategy());
  return laserBeam;
}

void EnemyLaserProjectile::initializeObjectType() {
  Projectile::initializeObjectType();
  m_objectTypes.insert(ObjectType::ENEMY_PROJECTILE);
}

void EnemyLaserProjectile::initializeSounds() {
  m_spawnSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_LASER});
}
} // namespace Projectiles
} // namespace GameObjects
