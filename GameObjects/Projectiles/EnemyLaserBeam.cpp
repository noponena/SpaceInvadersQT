#include "EnemyLaserBeam.h"

namespace GameObjects {
namespace Projectiles {

EnemyLaserBeam::EnemyLaserBeam(
    int damage, std::unordered_set<Weapons::ProjectileProperty> properties)
    : Projectile(damage, properties) {

  m_objectType = ObjectType::ENEMY_PROJECTILE;
  m_spawnSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_LASER});
  m_pixmapResourcePath = ":/Images/enemy_laser_projectile.png";
  m_pixmapScale = QPointF(29.055, 30.0);
}

std::unique_ptr<Projectile> EnemyLaserBeam::clone() const {
  std::unique_ptr<EnemyLaserBeam> laserBeam =
      std::make_unique<EnemyLaserBeam>();
  laserBeam->setDamage(m_damage);
  laserBeam->setProperties(m_properties);
  laserBeam->setMovementStrategy(this->movementStrategy());
  return laserBeam;
}

QPixmap EnemyLaserBeam::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

} // namespace Projectiles
} // namespace GameObjects
