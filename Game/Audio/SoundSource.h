#pragma once
#include <AL/al.h>

namespace Game {
namespace Audio {

class SoundSource {
public:
  SoundSource(const float &gain = 1.0f);
  ~SoundSource();

  void Play(const ALuint buffer_to_play);
  ALuint getSourceID() const;

private:
  ALuint p_Source;
  float p_Pitch = 1.f;
  float p_Position[3] = {0, 0, 0};
  float p_Velocity[3] = {0, 0, 0};
  bool p_LoopSound = false;
  ALuint p_Buffer = 0;
};

} // namespace Audio
} // namespace Game
