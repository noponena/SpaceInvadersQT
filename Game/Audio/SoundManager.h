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
#include <vector>

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
  void cleanup();
  float Gain = 1.0f;

private:
  SoundManager();
  ~SoundManager();

  void loadSounds();
  std::shared_ptr<SoundSource> getAvailableSource(float gain);

  std::vector<std::shared_ptr<SoundSource>> m_sourcePool;
  std::map<SoundEffect, std::pair<std::uint32_t, float>> m_sounds;
  std::list<std::shared_ptr<SoundSource>> m_activeSources;
  std::uint32_t m_soundCounter;
  std::uint32_t m_maxSoundCount;
};

} // namespace Audio
} // namespace Game

#endif // GAME_AUDIO_SOUNDMANAGER_H
