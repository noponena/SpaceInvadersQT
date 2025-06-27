#include "Health.h"
namespace GameObjects {
namespace Collectables {

Health::Health(const Transform &transform, const Config::GameContext ctx)
    : Collectable(transform, ctx) {
  RenderData normalData;
  normalData.size = QVector2D(22, 22);
  normalData.imagePath = ":/Images/health.png";
  addRenderData(RenderState::Normal, normalData);
}

void Health::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::HEALTH_COLLECTED});
}

void Health::initializeObjectType() {
  Collectable::initializeObjectType();
  m_objectTypes.insert(ObjectType::HEALTH);
}

std::unique_ptr<GameObject> Health::clone() const {
  std::unique_ptr<Health> health =
      std::make_unique<Health>(m_transform, m_gameContext);
  health->m_destructionSoundInfo = m_destructionSoundInfo;
  health->m_objectTypes = m_objectTypes;
  health->m_renderDataByState = m_renderDataByState;
  return health;
}

} // namespace Collectables
} // namespace GameObjects
