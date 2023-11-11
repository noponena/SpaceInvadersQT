#ifndef SOUNDINFO_H
#define SOUNDINFO_H

namespace Game {
namespace Audio {

enum class SoundEffect {
    LASER,
    LESSER_ENEMY_LASER,
    STELLAR_COIN_COLLECTED,
    PLAYER_DESTROYED,
    LESSER_ENEMY_DESTROYED,
};

struct SoundInfo {
    bool enabled = false;
    SoundEffect soundEffect;
    float gain = 1.0f;
};

}
}

#endif // SOUNDINFO_H
