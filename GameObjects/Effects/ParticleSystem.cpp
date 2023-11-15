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

ParticleSystem::ParticleSystem()
    : m_effectFinished(false) {}

void ParticleSystem::update(float deltaTimeInSeconds) {
  bool effectFinished = true;
  for (Particle &particle : m_particles) {
    particle.update(deltaTimeInSeconds);
    if (!particle.isDead())
        effectFinished = false;
  }

  m_effectFinished = effectFinished;
}

void ParticleSystem::spawnParticles(int count, QColor color,
                                    float lifespanInSeconds) {
  bool randomColor = color == nullptr;
  for (int i = 0; i < count; i++) {
    if (randomColor)
      color = this->randomColor();
    Particle particle(m_position, lifespanInSeconds, color);
    m_particles.push_back(particle);
  }
}

bool ParticleSystem::effectFinished() const
{
  return m_effectFinished;
}

void ParticleSystem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  if (!m_elapsedTimer.isValid()) {
    m_elapsedTimer.start();
  } else {
    this->update(static_cast<float>
                 (m_elapsedTimer.restart()) / 1000.0f);
  }
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
