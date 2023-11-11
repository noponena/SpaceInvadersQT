#include "BasicEnemyLaserBeam.h"

namespace GameObjects {
namespace Projectiles {

BasicEnemyLaser::BasicEnemyLaser(Position position,
                                 float speed, bool hostile,
                                 int damage, std::set<Weapons::WeaponProperty> properties)
    : LaserBeam(position, speed, hostile, damage, properties)
{
    m_spawnSoundInfo = Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_LASER});
    m_pixmapResourcePath = ":/Images/player_laser_projectile.png";
    m_pixmapScale = QPointF(29.055, 30.0);
}

} // namespace Projectiles
} // namespace GameObjects
