#include "ParticleSystem.h"
#include <QTimer>

namespace GameObjects {
namespace Effects {

QColor ParticleSystem::randomColor() {
  int r = QRandomGenerator::global()->bounded(256);
  int g = QRandomGenerator::global()->bounded(256);
  int b = QRandomGenerator::global()->bounded(256);

  return QColor(r, g, b);
}

void ParticleSystem::update() {
  for (Particle &particle : m_particles) {
    particle.update();
  }

  if (m_particles.empty()) {
    emit animationFinished();
  }
}

void ParticleSystem::spawnParticles(int count, QColor color,
                                    int lifespanFrames) {
  bool randomColor = color == nullptr;
  for (int i = 0; i < count; i++) {
    if (randomColor)
      color = this->randomColor();
    Particle particle(m_position, lifespanFrames, color);
    m_particles.push_back(particle);
  }
}

void ParticleSystem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  this->update();
  painter->setRenderHint(QPainter::Antialiasing, false);
  painter->save();
  painter->setPen(Qt::NoPen);
  for (Particle &particle : m_particles) {
    particle.draw(*painter);
  }
  painter->restore();
}

void ParticleSystem::setPosition(QPointF newPosition) {
  for (Particle &p : m_particles) {
    p.setPosition(newPosition);
  }

  m_position = newPosition;
}

} // namespace Effects
} // namespace GameObjects
