#ifndef GAME_AUDIO_SOUNDMANAGER_H
#define GAME_AUDIO_SOUNDMANAGER_H

#include "Game/Audio/SoundSource.h"
#include <Game/Audio/SoundInfo.h>
#include <Game/Audio/SoundDevice.h>
#include <cstdint>
#include <list>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace Game {
namespace Audio {

class SoundManager {
    std::queue<SoundInfo> soundQueue;
    std::mutex queueMutex;
    std::condition_variable queueCondition;
    bool isRunning = true;
    std::thread soundThread;

    void processSoundQueue();
public:
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    static SoundManager& getInstance() {
        static SoundManager instance;
        return instance;
    }

    void playSoundEffect(SoundInfo soundInfo);
    void playSoundEffectAsync(SoundInfo soundInfo);

private:
    SoundManager();
    ~SoundManager();

    void loadSounds();
    void cleanup();
    std::map<SoundEffect, uint32_t> m_sounds;
    std::list<std::shared_ptr<SoundSource>> m_activeSources;
    std::mutex m_activeSourcesMutex;
    unsigned m_soundCounter;
    unsigned m_maxSoundCount;
};

} // namespace Audio
} // namespace Game

#endif // GAME_AUDIO_SOUNDMANAGER_H
