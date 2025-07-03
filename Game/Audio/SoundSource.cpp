#include "SoundSource.h"
#include <QDebug>

namespace Game {
namespace Audio {

SoundSource::SoundSource(const float &gain) {
  alGenSources(1, &p_Source);
  alSourcef(p_Source, AL_PITCH, p_Pitch);
  alSourcef(p_Source, AL_GAIN, gain);
  alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1],
             p_Position[2]);
  alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1],
             p_Velocity[2]);
  alSourcei(p_Source, AL_LOOPING, p_LoopSound);
  alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundSource::~SoundSource() {
  alSourceStop(p_Source);
  alDeleteSources(1, &p_Source);
}

void SoundSource::Play(const ALuint buffer_to_play) {
  if (buffer_to_play != p_Buffer) {
    p_Buffer = buffer_to_play;
    alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
  }
  m_playStartTime = std::chrono::steady_clock::now();
  alSourcePlay(p_Source);
}

void SoundSource::reset(float gain) {
  alSourceStop(p_Source);
  alSourcef(p_Source, AL_GAIN, gain);
}

bool SoundSource::isPlaying() const {
  ALint state;
  alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

ALuint SoundSource::getSourceID() const { return p_Source; }

std::chrono::steady_clock::time_point SoundSource::getPlayStartTime() const {
  return m_playStartTime;
}
} // namespace Audio
} // namespace Game
