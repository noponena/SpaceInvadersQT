#include "Stellar.h"
#include "Game/Audio/SoundInfo.h"
#include "Graphics/PixmapLibrary.h"
#include "Graphics/PixmapRegistry.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Position &position) : Collectable(position) {
  m_pixmapData.pixmapResourcePath = ":/Images/coin.png";
  m_pixmapData.pixmapScale = QPointF(5.0, 5.0);
}

void Stellar::registerPixmaps() {
    Graphics::PixmapLibrary::getPixmap(":/Images/coin.png", 5.0, 5.0);
}

void Stellar::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::STELLAR_COIN_COLLECTED});
}

void Stellar::initializeObjectType() {
  Collectable::initializeObjectType();
  m_objectTypes.insert(ObjectType::STELLAR_COIN);
}

std::unique_ptr<GameObject> Stellar::clone() const {
  std::unique_ptr<Stellar> stellar = std::make_unique<Stellar>(m_position);
  stellar->m_pixmapData = m_pixmapData;
  stellar->m_destructionSoundInfo = m_destructionSoundInfo;
  stellar->m_objectTypes = m_objectTypes;
  return stellar;
}

namespace {
struct PixmapRegistrar {
    PixmapRegistrar() {
        PixmapRegistry::instance().add(&Stellar::registerPixmaps);
    }
};
static PixmapRegistrar _stellar_pixmap_registrar;
}

} // namespace Collectables
} // namespace GameObjects
