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

} // namespace Collectables
} // namespace GameObjects
