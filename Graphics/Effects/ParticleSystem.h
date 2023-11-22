#ifndef GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
#define GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H

#include "Effect.h"
#include "Particle.h"
#include <QElapsedTimer>
#include <QThread>

namespace Graphics {
namespace Effects {

class ParticleSystem : public Effect {
  Q_OBJECT
public:
  ParticleSystem();
  ~ParticleSystem();
  void update(float deltaTimeInSeconds);
  void spawnParticles(int count, QColor color = nullptr,
                      float lifespanInSeconds = 1.0f);

  operator bool() const { return !m_particles.empty(); }

private:
  QThread m_updateThread;
  bool m_effectFinished;
  bool m_particlesUpdated;
  bool m_particlesDrawn;
  bool m_updateThreadStarted;
  std::vector<Particle> m_particles;
  QElapsedTimer m_elapsedTimer;
  QColor getRandomColor();

  void runUpdateLoop();
  public:
  QRectF boundingRect() const override { return QRectF(); };
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

  // Effect interface
public:
  void setPosition(QPointF newPosition) override;
    bool effectFinished() const;
};

} // namespace Effects
} // namespace Graphics

#endif // GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
