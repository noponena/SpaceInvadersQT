#include "SoundManager.h"
#include "Game/Audio/SoundBuffer.h"
#include "Game/Audio/SoundDevice.h"
#include "Game/Audio/SoundSource.h"
#include <QDebug>

namespace Game {
namespace Audio {

SoundManager::SoundManager() : m_soundCounter(0), m_maxSoundCount(50) {
  loadSounds();
}

SoundManager::~SoundManager() {}

void SoundManager::playSoundEffect(SoundInfo soundInfo) {
  if (soundInfo.enabled) {
    std::pair<std::uint32_t, float> sound = m_sounds[soundInfo.soundEffect];
    float gain = Gain * sound.second * soundInfo.gain;
    std::shared_ptr<SoundSource> source = getAvailableSource(gain);
    if (source) {
      m_activeSources.push_back(source);
      source->Play(sound.first);
      m_soundCounter++;
    }
  }
}

std::shared_ptr<SoundSource> SoundManager::getAvailableSource(float gain) {
  // 1. Look for a stopped source
  for (auto &src : m_sourcePool) {
    if (!src->isPlaying()) {
      src->reset(gain);
      return src;
    }
  }
  // 2. If all are busy, but pool isn't full, create a new one
  if (m_sourcePool.size() < m_maxSoundCount) {
    auto src = std::make_shared<SoundSource>(gain);
    m_sourcePool.push_back(src);
    return src;
  }
  // 3. Pool is full and all busy: steal/override an existing source
  // Example: steal the oldest
  auto &toSteal =
      *std::min_element(m_sourcePool.begin(), m_sourcePool.end(),
                        [](const auto &a, const auto &b) {
                          return a->getPlayStartTime() < b->getPlayStartTime();
                        });
  toSteal->reset(gain);
  return toSteal;
}

void SoundManager::loadSounds() {
  SoundDevice::get();

  std::uint32_t laser =
      SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/laser.wav");
  std::uint32_t lesserEnemyLaser =
      SoundBuffer::get()->addSoundEffectFromResource(
          ":/Sounds/lesser_enemy_laser.wav");
  std::uint32_t vortexCannon = SoundBuffer::get()->addSoundEffectFromResource(
      ":/Sounds/vortex_cannon.wav");

  std::uint32_t playerDestroyed =
      SoundBuffer::get()->addSoundEffectFromResource(
          ":/Sounds/player_explosion.wav");

  std::uint32_t lesserEnemyDestroyed =
      SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/explosion.wav");
  std::uint32_t stellarCoinCollected =
      SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/collect.wav");
  std::uint32_t healthCoinCollected =
      SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/health.wav");

  SoundSource src;

  m_sounds[SoundEffect::LASER] = std::make_pair(laser, 0.02f);
  m_sounds[SoundEffect::LESSER_ENEMY_LASER] =
      std::make_pair(lesserEnemyLaser, 0.025f);
  m_sounds[SoundEffect::VORTEX_CANNON] = std::make_pair(vortexCannon, 1.0f);

  m_sounds[SoundEffect::PLAYER_DESTROYED] =
      std::make_pair(playerDestroyed, 1.0f);
  m_sounds[SoundEffect::LESSER_ENEMY_DESTROYED] =
      std::make_pair(lesserEnemyDestroyed, 1.0f);

  m_sounds[SoundEffect::STELLAR_COIN_COLLECTED] =
      std::make_pair(stellarCoinCollected, 1.0f);
  m_sounds[SoundEffect::HEALTH_COLLECTED] =
      std::make_pair(healthCoinCollected, 1.0f);
}

void SoundManager::cleanup() {
  for (auto it = m_activeSources.begin(); it != m_activeSources.end();) {
    ALint state;
    alGetSourcei((*it)->getSourceID(), AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
      it = m_activeSources.erase(it);
    } else {
      ++it;
    }
  }

  m_soundCounter = 0;
}

} // namespace Audio
} // namespace Game
