#include "LaserBeam.h"
#include "Game/Audio/SoundInfo.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
LaserBeam::LaserBeam(bool hostile, int damage, std::set<Weapons::ProjectileProperty> properties)
    : Projectile(hostile, damage, properties)
{
    m_spawnSoundInfo = Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::LASER});
    m_pixmapResourcePath = ":/Images/player_laser_projectile.png";
    m_pixmapScale = QPointF(29.055, 30.0);
}

std::unique_ptr<Projectile> LaserBeam::clone() const
{
    std::unique_ptr<LaserBeam> laserBeam = std::make_unique<LaserBeam>();
    laserBeam->setHostile(m_hostile);
    laserBeam->setDamage(m_damage);
    laserBeam->setProperties(m_properties);
    laserBeam->setMovementStrategy(this->movementStrategy());
    return laserBeam;
}

QPixmap LaserBeam::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
    return pixmap;
}

} // namespace Projectiles
} // namespace GameObjects
