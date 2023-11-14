#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H

#include <QThread>
#include <QSoundEffect>
#include <QEventLoop>

namespace GameObjects
{
struct SoundInfo;
}

namespace Game {
namespace Audio {
class SoundThread : public QThread {
    Q_OBJECT
public:
    void run() override;

    void setSoundInfo(GameObjects::SoundInfo *newSoundInfo);

private:
    GameObjects::SoundInfo *m_soundInfo;
};

}
}


#endif // SOUNDTHREAD_H
