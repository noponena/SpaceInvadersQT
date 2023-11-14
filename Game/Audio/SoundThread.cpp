#include "SoundThread.h"
#include "GameObjects/GameObject.h"

namespace Game {
namespace Audio {
void SoundThread::run()
{
    QSoundEffect effect;
    effect.setSource(m_soundInfo->url);
    effect.setVolume(1.0f);
    effect.play();
    QThread::msleep(m_soundInfo->soundLengthMs);
    delete m_soundInfo;
}

void SoundThread::setSoundInfo(GameObjects::SoundInfo *newSoundInfo)
{
    m_soundInfo = newSoundInfo;
}

}
}
