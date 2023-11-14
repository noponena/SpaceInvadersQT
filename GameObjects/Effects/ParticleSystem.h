#ifndef GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
#define GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H

#include "Effect.h"
#include "Particle.h"

namespace GameObjects {
namespace Effects {

class ParticleSystem : public Effect {
  Q_OBJECT
public:
  void update();
  void spawnParticles(int count, QColor color = nullptr,
                      int lifespanFrames = 20);

private:
  std::vector<Particle> m_particles;
  QColor randomColor();

public:
  QRectF boundingRect() const override { return QRectF(); };
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
signals:
  void animationFinished();

  // Effect interface
public:
  void setPosition(QPointF newPosition) override;
};

} // namespace Effects
} // namespace GameObjects

#endif // GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
