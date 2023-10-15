#ifndef GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
#define GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H

#include "Effect.h"
#include "Particle.h"


namespace GameObjects {
namespace Effects {

class ParticleSystem : public Effect {
    Q_OBJECT
public:
    ParticleSystem(QPointF position);
    void update();
    void draw(QPainter &painter);
    void spawnParticles(int count, QColor color = nullptr, int lifespanFrames = 60);
    void start();

private:
    std::list<Particle> m_particles;
    QColor randomColor();
    QTimer *m_updateTimer;
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
signals:
    void animationFinished();
};

} // namespace Effects
} // namespace GameObjects

#endif // GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
