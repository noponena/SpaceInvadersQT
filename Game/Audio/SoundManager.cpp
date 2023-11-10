#include "SoundManager.h"
#include "Game/Audio/SoundDevice.h"
#include "Game/Audio/SoundBuffer.h"
#include "Game/Audio/SoundSource.h"
#include <QDebug>
#include <QtConcurrent>

namespace Game {
namespace Audio {

SoundManager::SoundManager() : m_soundCounter(0), m_maxSoundCount(50)
{
    this->loadSounds();
}

SoundManager::~SoundManager()
{
    isRunning = false;
    queueCondition.notify_one();
    soundThread.join();
}

void SoundManager::processSoundQueue()
{
    while (isRunning) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [&] { return !soundQueue.empty() || !isRunning; });

        if (!isRunning) break;

        SoundInfo soundInfo = soundQueue.front();
        soundQueue.pop();
        lock.unlock();

        // Now play the sound using SoundSource
        SoundSource speaker;
        uint32_t sound = m_sounds[soundInfo.soundEffect];
        speaker.Play(sound);
    }
}

void SoundManager::playSoundEffect(SoundInfo soundInfo)
{
    if (soundInfo.enabled) {
        if (m_soundCounter >= m_maxSoundCount)
            this->cleanup();
        std::shared_ptr<SoundSource> source = std::make_shared<SoundSource>();
        m_activeSources.push_back(source);
        uint32_t sound = m_sounds[soundInfo.soundEffect];
        source->Play(sound);
        m_soundCounter++;
    }
}

void SoundManager::playSoundEffectAsync(SoundInfo soundInfo)
{
    if (soundInfo.enabled) {
        std::lock_guard<std::mutex> lock(queueMutex);
        soundQueue.push(soundInfo);
        queueCondition.notify_one();
    }
}

void SoundManager::loadSounds()
{
    SoundDevice::get();
    uint32_t laser = SoundBuffer::get()->addSoundEffect("C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Sounds/laser.wav");
    uint32_t lesserEnemyDestroyed = SoundBuffer::get()->addSoundEffect("C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Sounds/explosion.wav");
    uint32_t stellarCoinCollected = SoundBuffer::get()->addSoundEffect("C:/Users/aaron/OneDrive/Tiedostot/Aaro/Personal/Projects/QT/SpaceInvadersQT/Sounds/collect.wav");
    SoundSource src;
    m_sounds[LASER] = laser;
    m_sounds[LESSER_ENEMY_DESTROYED] = lesserEnemyDestroyed;
    m_sounds[STELLAR_COIN_COLLECTED] = stellarCoinCollected;
}


void SoundManager::cleanup() {
    std::lock_guard<std::mutex> lock(m_activeSourcesMutex);
    qDebug() << "cleanup!";
    for (auto it = m_activeSources.begin(); it != m_activeSources.end(); ) {
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
