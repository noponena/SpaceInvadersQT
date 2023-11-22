#include "WaveOfDestruction.h"

namespace GameObjects {
namespace Projectiles {

WaveOfDestruction::WaveOfDestruction()
{

}

bool WaveOfDestruction::shouldBeDeleted()
{
    return m_position.isBeyondScreenBottomLimit(50) ||
           m_position.isBeyondScreenTopLimit(50);
}

std::unique_ptr<Projectile> WaveOfDestruction::clone() const
{
    std::unique_ptr<WaveOfDestruction> projectile =
        std::make_unique<WaveOfDestruction>();
    projectile->m_objectTypes = m_objectTypes;
    projectile->m_magnetism = m_magnetism;
    projectile->setSpawnSoundInfo(m_spawnSoundInfo);
    projectile->setDestructionSoundInfo(m_destructionSoundInfo);
    projectile->setDamage(m_damage);
    projectile->setProperties(m_properties);
    projectile->setPixmapData(m_pixmapData);
    projectile->setMovementStrategy(movementStrategy());
    return projectile;
}

} // namespace Projectiles
} // namespace GameObjects
