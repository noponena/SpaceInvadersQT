#include "SoundDevice.h"
#include <AL/al.h>
#include <qlogging.h>
#include <stdio.h>

namespace Game {
namespace Audio {

SoundDevice *SoundDevice::get() {
  static SoundDevice *snd_device = new SoundDevice();
  return snd_device;
}

SoundDevice::SoundDevice() {
  p_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
  if (!p_ALCDevice)
    throw("failed to get sound device");

  p_ALCContext = alcCreateContext(p_ALCDevice, nullptr); // create context
  if (!p_ALCContext)
    throw("Failed to set sound context");

  if (!alcMakeContextCurrent(p_ALCContext)) // make context current
    throw("failed to make context current");

  const ALCchar *name = nullptr;
  if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
    name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
  if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
    name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
  printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice() {
  if (!alcMakeContextCurrent(nullptr))
    qWarning("SoundDevice: failed to set context to nullptr");

  alcDestroyContext(p_ALCContext);
  if (p_ALCContext)
    qWarning("SoundDevice: failed to unset context during close");

  if (!alcCloseDevice(p_ALCDevice))
    qWarning("SoundDevice: failed to close sound device");
}
} // namespace Audio
} // namespace Game
