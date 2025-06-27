#include "WaveOfDestruction.h"

namespace GameObjects {
namespace Projectiles {

WaveOfDestruction::WaveOfDestruction(Config::GameContext ctx)
    : Projectile(ctx) {
  m_spawnSoundInfo.enabled = false;

  RenderData normalData;
  normalData.size = QVector2D(250, 20);
  normalData.imagePath = ":/Images/wave.png";
  addRenderData(RenderState::Normal, normalData);

  /*
  RenderData onHitData;
  onHitData.size = QVector2D(100, 100);
  onHitData.imagePath = ":/Images/black_hole.png";
  addRenderData(RenderState::OnHit, onHitData);


  RenderData hudData;
  hudData.size = QVector2D(25, 25);
  hudData.imagePath = ":/Images/wave_of_destruction_hud.png";
  addRenderData(RenderState::Hud, hudData);
 */
}

bool WaveOfDestruction::shouldBeDeleted() {
  return Utils::BoundsChecker::isBeyondScreenBottom(
             m_transform.position, m_gameContext.movementBounds, 50) ||
         Utils::BoundsChecker::isBeyondScreenTop(
             m_transform.position, m_gameContext.movementBounds, 50);
}

std::unique_ptr<GameObject> WaveOfDestruction::clone() const {
  std::unique_ptr<WaveOfDestruction> projectile =
      std::make_unique<WaveOfDestruction>(m_gameContext);
  projectile->m_objectTypes = m_objectTypes;
  projectile->m_magnetism = m_magnetism;
  projectile->setSpawnSoundInfo(m_spawnSoundInfo);
  projectile->setDestructionSoundInfo(m_destructionSoundInfo);
  projectile->setDamage(m_damage);
  projectile->setProperties(m_properties);
  projectile->setRenderDataByState(renderDataByState());
  projectile->setMovementStrategy(movementStrategy());
  return projectile;
}

} // namespace Projectiles
} // namespace GameObjects
