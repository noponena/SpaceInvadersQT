#pragma once
#include <AL/al.h>
#include <chrono>

namespace Game {
namespace Audio {

class SoundSource {
public:
  SoundSource(const float &gain = 1.0f);
  ~SoundSource();

  void Play(const ALuint buffer_to_play);
  bool isPlaying() const;
  void reset(float gain);
  ALuint getSourceID() const;

  std::chrono::steady_clock::time_point getPlayStartTime() const;

private:
  std::chrono::steady_clock::time_point m_playStartTime;
  ALuint p_Source;
  float p_Pitch = 1.f;
  float p_Position[3] = {0, 0, 0};
  float p_Velocity[3] = {0, 0, 0};
  bool p_LoopSound = false;
  ALuint p_Buffer = 0;
};

} // namespace Audio
} // namespace Game
