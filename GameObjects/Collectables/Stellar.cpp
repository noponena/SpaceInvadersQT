#include "Stellar.h"
#include "Game/Audio/SoundInfo.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Position &position) : Collectable(position) {
  m_pixmapResourcePath = ":/Images/coin.png";
  m_pixmapScale = QPointF(5.0, 5.0);
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::STELLAR_COIN_COLLECTED});
}

QPixmap Stellar::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

void Stellar::collideWith(GameObject &other) {
  other.collideWithStellarToken(*this);
}

} // namespace Collectables
} // namespace GameObjects
