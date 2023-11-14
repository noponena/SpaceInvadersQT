#include "BasicEnemyLaserBeam.h"

namespace GameObjects {
namespace Projectiles {

LesserEnemyLaserBeam::LesserEnemyLaserBeam(bool hostile,
                                 int damage, std::set<Weapons::ProjectileProperty> properties)
    : LaserBeam(hostile, damage, properties)
{
    m_spawnSoundInfo = Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_LASER});
    m_pixmapResourcePath = ":/Images/player_laser_projectile.png";
    m_pixmapScale = QPointF(29.055, 30.0);
}

} // namespace Projectiles
} // namespace GameObjects
