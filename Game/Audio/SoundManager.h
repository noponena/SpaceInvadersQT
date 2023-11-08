#ifndef GAME_AUDIO_SOUNDMANAGER_H
#define GAME_AUDIO_SOUNDMANAGER_H

namespace GameObjects {
struct SoundInfo;
}

namespace Game {
namespace Audio {

class SoundManager
{
public:
    SoundManager();
    void playSoundEffect(const GameObjects::SoundInfo soundInfo);
};

} // namespace Audio
} // namespace Game

#endif // GAME_AUDIO_SOUNDMANAGER_H
