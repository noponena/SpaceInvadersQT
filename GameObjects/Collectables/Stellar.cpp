#include "Stellar.h"
#include "Game/Audio/SoundInfo.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Config::GameContext &ctx) : Collectable(ctx) {
  RenderData normalData;
  normalData.size = QVector2D(5, 5);
  normalData.imagePath = ":/Images/coin.png";
  addRenderData(State::Normal, normalData);
  m_transform.colliderSize = {5, 5};
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
  std::unique_ptr<Stellar> stellar = std::make_unique<Stellar>(m_gameContext);
  stellar->setTransform(m_transform);
  stellar->m_destructionSoundInfo = m_destructionSoundInfo;
  stellar->m_objectTypes = m_objectTypes;
  stellar->m_renderDataByState = m_renderDataByState;
  return stellar;
}

} // namespace Collectables
} // namespace GameObjects
