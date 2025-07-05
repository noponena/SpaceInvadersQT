#include "Vortex.h"

namespace GameObjects {
namespace Projectiles {

Vortex::Vortex(Config::GameContext ctx)
    : Projectile(ctx), m_timeToLiveSeconds(5.0f),
      m_timeSinceSpawnSeconds(0.0f) {
  m_magnetism = Magnetism{true, false, 200, 200};
  m_spawnSoundInfo =
      Game::Audio::SoundInfo({true, Game::Audio::SoundEffect::VORTEX_CANNON});

  RenderData normalData;
  normalData.size = QVector2D(25, 25);
  normalData.imagePath = ":/Images/black_orb.png";
  addRenderData(State::Normal, normalData);

  RenderData onHitData;
  onHitData.size = QVector2D(100, 100);
  onHitData.imagePath = ":/Images/black_hole.png";
  addRenderData(State::OnHit, onHitData);

  m_hudPixmapResourcePath = ":/Images/black_hole_hud.png";

  /*
  RenderData hudData;
  hudData.size = QVector2D(25, 25);
  hudData.imagePath = ":/Images/black_hole_hud.png";
  addRenderData(State::Hud, hudData);
 */
}

void Vortex::update(const UpdateContext &context) {
  Projectile::update(context);
  m_timeSinceSpawnSeconds += context.deltaTimeInSeconds;
}

void Vortex::collideWithEnemyShip(Ships::EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  m_magnetism.enabled = true;
  m_collidable = false;
  setState(State::OnHit);
  disableMovement();
}

bool Vortex::shouldBeDeleted() {
  return GameObject::shouldBeDeleted() ||
         m_timeSinceSpawnSeconds >= m_timeToLiveSeconds;
}

std::unique_ptr<GameObject> Vortex::clone() const {
  std::unique_ptr<Vortex> projectile = std::make_unique<Vortex>(m_gameContext);
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
