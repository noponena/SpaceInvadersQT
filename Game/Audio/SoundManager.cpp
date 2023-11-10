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

}

void SoundManager::playSoundEffect(SoundInfo soundInfo)
{
    if (soundInfo.enabled) {
        if (m_soundCounter >= m_maxSoundCount)
            this->cleanup();
        std::pair<uint32_t, float> sound = m_sounds[soundInfo.soundEffect];
        float gain = Gain * sound.second * soundInfo.gain;
        std::shared_ptr<SoundSource> source = std::make_shared<SoundSource>(gain);
        m_activeSources.push_back(source);
        source->Play(sound.first);
        m_soundCounter++;
    }
}

void SoundManager::loadSounds()
{
    SoundDevice::get();
    uint32_t laser = SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/laser.wav");
    uint32_t lesserEnemyDestroyed = SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/explosion.wav");
    uint32_t stellarCoinCollected = SoundBuffer::get()->addSoundEffectFromResource(":/Sounds/collect.wav");
    SoundSource src;
    m_sounds[LASER] = std::make_pair(laser, 0.2f);
    m_sounds[LESSER_ENEMY_DESTROYED] = std::make_pair(lesserEnemyDestroyed, 1.0f);
    m_sounds[STELLAR_COIN_COLLECTED] = std::make_pair(stellarCoinCollected, 1.0f);
}


void SoundManager::cleanup() {
    std::lock_guard<std::mutex> lock(m_activeSourcesMutex);
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
