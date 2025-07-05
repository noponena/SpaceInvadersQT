#ifndef PARTICLE_H
#define PARTICLE_H

#include <QDebug>
#include <QPainter>
#include <QPointF>
#include <QRandomGenerator>
#include <QVector2D>

namespace Graphics {
namespace Effects {

struct Particle {
  QVector2D position;
  QVector2D velocity;
  float life;     // Time left
  float lifeSpan; // Initial life
  QColor color;

  Particle(const QVector2D &pos, float lifespan, const QColor &col,
           float maxSpeed)
      : position(pos), life(lifespan), lifeSpan(lifespan), color(col) {
    float angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
    float speed = QRandomGenerator::global()->generateDouble() * maxSpeed;
    velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
  }

  void update(float dt) {
    position += velocity * dt;
    life -= dt;
  }
  bool isAlive() const { return life > 0.0f; }
};

} // namespace Effects
} // namespace Graphics

#endif // PARTICLE_H
