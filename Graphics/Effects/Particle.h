#ifndef PARTICLE_H
#define PARTICLE_H

#include "Utils/Math/MathConstants.h"
#include <QDebug>
#include <QPainter>
#include <QPointF>
#include <QRandomGenerator>

namespace Graphics {
namespace Effects {
class Particle {
public:
  Particle(QPointF position, float lifespanInSeconds, QColor color)
      : m_position(position), m_lifeSpanInSeconds(lifespanInSeconds),
        m_lifeSpanLeft(lifespanInSeconds), m_color(color) {
    m_velocity = randomVelocity(5000);
  };

  QPointF randomVelocity(float maxSpeed) {
    float angle = QRandomGenerator::global()->generateDouble() * 2 * PI;
    float speed = QRandomGenerator::global()->generateDouble() * maxSpeed;

    return QPointF(cos(angle) * speed, sin(angle) * speed);
  }

  void move(float deltaTimeInSeconds) {
    m_position += m_velocity * deltaTimeInSeconds;
  };
  bool isDead() const { return m_lifeSpanLeft <= 0; };

  void draw(QPainter &painter) {

    QColor currentColor = m_color;
    currentColor.setAlphaF(m_lifeSpanLeft / m_lifeSpanInSeconds);
    painter.setBrush(currentColor);
    QRectF rect(m_position, m_position + QPointF(1, 1));
    painter.drawRect(rect);
  }

  void update(float deltaTimeInSeconds) {
    move(deltaTimeInSeconds);
    m_lifeSpanLeft -= deltaTimeInSeconds;
    if (m_lifeSpanLeft < 0)
      m_lifeSpanLeft = 0;
  }

  QPointF position() const;
  void setPosition(QPointF newPosition);

private:
  QPointF m_position;
  QPointF m_velocity;
  float m_lifeSpanInSeconds;
  float m_lifeSpanLeft;
  QColor m_color;
};

inline QPointF Particle::position() const { return m_position; }

inline void Particle::setPosition(QPointF newPosition) {
  m_position = newPosition;
}

} // namespace Effects
} // namespace Graphics

#endif // PARTICLE_H
