#ifndef GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
#define GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H

#include "Effect.h"
#include "Particle.h"
#include <QElapsedTimer>

namespace GameObjects {
namespace Effects {

class ParticleSystem : public Effect {
  Q_OBJECT
public:
  ParticleSystem();
  void update(float deltaTimeInSeconds);
  void spawnParticles(int count, QColor color = nullptr,
                      float lifespanInSeconds = 1.0f);

private:
  bool m_effectFinished;
  std::vector<Particle> m_particles;
  QElapsedTimer m_elapsedTimer;
  QColor randomColor();

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
} // namespace GameObjects

#endif // GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
