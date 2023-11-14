#ifndef PARTICLE_H
#define PARTICLE_H

#include <QDebug>
#include <QPainter>
#include <QPointF>
#include <QRandomGenerator>

namespace GameObjects {
namespace Effects {
class Particle {
public:
  Particle(QPointF position, int lifespan, QColor color)
      : m_position(position), m_lifespan(lifespan), m_maxLifespan(lifespan),
        m_color(color) {
    m_velocity = randomVelocity(50);
  };

  QPointF randomVelocity(float maxSpeed) {
    float angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
    float speed = QRandomGenerator::global()->generateDouble() * maxSpeed;

    return QPointF(cos(angle) * speed, sin(angle) * speed);
  }

  void move() { m_position += m_velocity; };
  bool isDead() const { return m_lifespan <= 0; };

  void draw(QPainter &painter) {

    QColor currentColor = m_color;
    int alpha = static_cast<int>(255.0 * static_cast<double>(m_lifespan) /
                                 static_cast<double>(m_maxLifespan));
    currentColor.setAlpha(alpha);
    painter.setBrush(currentColor);

    QRectF rect(m_position, m_position + QPointF(1, 1));
    painter.drawRect(rect);
  }

  void update() {
    this->move();
    m_lifespan -= 1;
    if (m_lifespan < 0)
      m_lifespan = 0;
  }

  QPointF position() const;
  void setPosition(QPointF newPosition);

private:
  QPointF m_position;
  QPointF m_velocity;
  int m_lifespan;
  int m_maxLifespan;
  QColor m_color;
};

inline QPointF Particle::position() const { return m_position; }

inline void Particle::setPosition(QPointF newPosition) {
  m_position = newPosition;
}

} // namespace Effects
} // namespace GameObjects

#endif // PARTICLE_H
