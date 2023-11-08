#include "SoundManager.h"
#include "GameObjects/GameObject.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QtConcurrent/QtConcurrentRun>

namespace Game {
namespace Audio {

SoundManager::SoundManager()
{
    this->playSoundEffect(GameObjects::SoundInfo({true, QUrl("qrc:/Sounds/laser.wav"), 0}));
}

void SoundManager::playSoundEffect(const GameObjects::SoundInfo soundInfo)
{
    if (soundInfo.enabled) {
        QtConcurrent::run([soundInfo] {
            QMediaPlayer player;
            QAudioOutput audioOutput;
            player.setAudioOutput(&audioOutput);
            player.setSource(soundInfo.url);
            player.play();
            QThread::msleep(soundInfo.soundLengthMs);
        });
    }
}

} // namespace Audio
} // namespace Game
