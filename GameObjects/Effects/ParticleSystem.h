#ifndef GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
#define GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H

#include <QGraphicsItem>
#include "Particle.h"


namespace GameObjects {
namespace Effects {

class ParticleSystem : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    ParticleSystem(QPointF position);
    void update();
    void draw(QPainter &painter);
    void spawnParticles(int count, QColor color = nullptr, int lifespanFrames = 60);
    void start();

private:
    std::list<Particle> m_particles;
    QPointF m_position;
    QColor randomColor();
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

} // namespace Effects
} // namespace GameObjects

#endif // GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
