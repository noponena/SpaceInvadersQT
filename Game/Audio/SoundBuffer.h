#pragma once
#include <AL/al.h>
#include <QString>
#include <vector>

namespace Game {
namespace Audio {

class SoundBuffer {
public:
  static SoundBuffer *get();

  ALuint addSoundEffectFromResource(const QString &resourcePath);
  ALuint addSoundEffect(const char *filename);
  bool removeSoundEffect(const ALuint &buffer);

private:
  SoundBuffer();
  ~SoundBuffer();

  std::vector<ALuint> p_SoundEffectBuffers;
};

} // namespace Audio
} // namespace Game
