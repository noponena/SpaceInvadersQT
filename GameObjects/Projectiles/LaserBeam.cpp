#include "LaserBeam.h"
#include "Game/Audio/SoundInfo.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
LaserBeam::LaserBeam(Position position, float speed, bool hostile, int damage, std::set<Weapons::WeaponProperty> properties)
    : Projectile(position, speed, hostile, damage, properties)
{
    m_spawnSoundInfo = Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::LASER});
    m_pixmapResourcePath = ":/Images/player_laser_projectile.png";
    m_pixmapScale = QPointF(29.055, 30.0);
}

QPixmap LaserBeam::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
    return pixmap;
}

} // namespace Projectiles
} // namespace GameObjects
