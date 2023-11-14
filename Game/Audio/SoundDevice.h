#pragma once
#include <AL/alc.h>

namespace Game {
namespace Audio {
class SoundDevice {
public:
  static SoundDevice *get();

private:
  SoundDevice();
  ~SoundDevice();

  ALCdevice *p_ALCDevice;
  ALCcontext *p_ALCContext;
};
} // namespace Audio
} // namespace Game
