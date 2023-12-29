#include "WaveOfDestruction.h"

namespace GameObjects {
namespace Projectiles {

WaveOfDestruction::WaveOfDestruction() {
  m_spawnSoundInfo.enabled = false;
  m_pixmapData.pixmapResourcePath = ":/Images/wave.png";
  m_pixmapData.hudPixmapResourcePath = ":/Images/wave_of_destruction_hud.png";
  m_pixmapData.pixmapScale = QPointF(250, 20);
  m_pixmapData.keepAspectRatio = false;
}

bool WaveOfDestruction::shouldBeDeleted() {
  return m_position.isBeyondScreenBottomLimit(50) ||
         m_position.isBeyondScreenTopLimit(50);
}

std::unique_ptr<GameObject> WaveOfDestruction::clone() const {
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
