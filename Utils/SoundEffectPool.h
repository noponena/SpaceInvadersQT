#ifndef SOUNDEFFECTPOOL_H
#define SOUNDEFFECTPOOL_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QList>
#include <QUrl>

class SoundEffectPool : public QObject {
    Q_OBJECT

public:
    SoundEffectPool(QObject *parent = nullptr) : QObject(parent) {}

    void playSound(const QUrl &soundUrl) {
        QMediaPlayer *player = findAvailablePlayer();
        if (!player) {
            player = createPlayer(soundUrl);
        }

        player->play();
    }

private:
    QList<QMediaPlayer*> players;

    QMediaPlayer *findAvailablePlayer() {
        for (QMediaPlayer *player : players) {
            if (player->playbackState() == QMediaPlayer::StoppedState) {
                return player;
            }
        }
        return nullptr; // No available player found
    }

    QMediaPlayer *createPlayer(const QUrl &soundUrl) {
        auto player = new QMediaPlayer(this);
        auto audioOutput = new QAudioOutput(this);
        player->setAudioOutput(audioOutput);
        player->setSource(soundUrl);
        players.append(player);
        return player;
    }
};

#endif // SOUNDEFFECTPOOL_H
