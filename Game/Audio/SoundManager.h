#ifndef GAME_AUDIO_SOUNDMANAGER_H
#define GAME_AUDIO_SOUNDMANAGER_H

#include "Game/Audio/SoundSource.h"
#include <Game/Audio/SoundDevice.h>
#include <Game/Audio/SoundInfo.h>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <mutex>

namespace Game {
namespace Audio {

class SoundManager {
public:
  SoundManager(const SoundManager &) = delete;
  SoundManager &operator=(const SoundManager &) = delete;

  static SoundManager &getInstance() {
    static SoundManager instance;
    return instance;
  }

  void playSoundEffect(SoundInfo soundInfo);
  float Gain = 1.0f;

private:
  SoundManager();
  ~SoundManager();

  void loadSounds();
  void cleanup();
  std::map<SoundEffect, std::pair<std::uint32_t, float>> m_sounds;
  std::list<std::shared_ptr<SoundSource>> m_activeSources;
  std::mutex m_activeSourcesMutex;
  std::uint32_t m_soundCounter;
  std::uint32_t m_maxSoundCount;
};

} // namespace Audio
} // namespace Game

#endif // GAME_AUDIO_SOUNDMANAGER_H
