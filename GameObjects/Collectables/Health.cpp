#include "Health.h"

namespace GameObjects {
namespace Collectables {

Health::Health(const Position &position) : Collectable(position) {
  m_pixmapData.pixmapResourcePath = ":/Images/health.png";
  m_pixmapData.pixmapScale = QPointF(22.0, 22.0);
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
  std::unique_ptr<Health> health = std::make_unique<Health>(m_position);
  health->m_pixmapData = m_pixmapData;
  health->m_destructionSoundInfo = m_destructionSoundInfo;
  health->m_objectTypes = m_objectTypes;
  return health;
}

} // namespace Collectables
} // namespace GameObjects
