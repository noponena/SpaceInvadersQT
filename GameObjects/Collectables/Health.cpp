#include "Health.h"

namespace GameObjects {
namespace Collectables {

Health::Health(const Position &position) : Collectable(position) {}

void Health::initializeGraphics() {
  m_pixmapResourcePath = ":/Images/health.png";
  m_pixmapScale = QPointF(20.0, 20.0);
}

void Health::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::HEALTH_COLLECTED});
}

void Health::initializeObjectType() { m_objectType = ObjectType::HEALTH; }

} // namespace Collectables
} // namespace GameObjects
