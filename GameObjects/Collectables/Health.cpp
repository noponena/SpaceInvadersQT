#include "Health.h"
namespace GameObjects {
namespace Collectables {

Health::Health(const Config::GameContext &ctx) : Collectable(ctx) {
  RenderData normalData;
  normalData.size = QVector2D(22, 22);
  normalData.imagePath = ":/Images/health.png";
  addRenderData(State::Normal, normalData);
  m_transform.colliderSize = {5, 5};
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
  std::unique_ptr<Health> health = std::make_unique<Health>(m_gameContext);
  health->setTransform(m_transform);
  health->m_destructionSoundInfo = m_destructionSoundInfo;
  health->m_objectTypes = m_objectTypes;
  health->m_renderDataByState = m_renderDataByState;
  return health;
}

} // namespace Collectables
} // namespace GameObjects
