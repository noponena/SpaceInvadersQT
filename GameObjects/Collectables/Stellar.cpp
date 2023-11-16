#include "Stellar.h"
#include "Game/Audio/SoundInfo.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Position &position) : Collectable(position) {}

void Stellar::collideWith(GameObject &other) {
  other.collideWithStellarToken(*this);
}

void Stellar::initializeGraphics() {
  m_pixmapResourcePath = ":/Images/coin.png";
  m_pixmapScale = QPointF(5.0, 5.0);
}

void Stellar::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::STELLAR_COIN_COLLECTED});
}

} // namespace Collectables
} // namespace GameObjects
