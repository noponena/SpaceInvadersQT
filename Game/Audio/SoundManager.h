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
  float Gain = 0.1f;

private:
  SoundManager();
  ~SoundManager();

  void loadSounds();
  void cleanup();
  std::map<SoundEffect, std::pair<uint32_t, float>> m_sounds;
  std::list<std::shared_ptr<SoundSource>> m_activeSources;
  std::mutex m_activeSourcesMutex;
  unsigned m_soundCounter;
  unsigned m_maxSoundCount;
};

} // namespace Audio
} // namespace Game

#endif // GAME_AUDIO_SOUNDMANAGER_H
