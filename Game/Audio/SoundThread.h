#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H

#include <QEventLoop>
#include <QSoundEffect>
#include <QThread>

namespace GameObjects {
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

} // namespace Audio
} // namespace Game

#endif // SOUNDTHREAD_H
