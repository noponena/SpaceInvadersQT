#include "ParticleSystem.h"

namespace Graphics {
namespace Effects {

QColor ParticleSystem::getRandomColor() {
  int r = QRandomGenerator::global()->bounded(256);
  int g = QRandomGenerator::global()->bounded(256);
  int b = QRandomGenerator::global()->bounded(256);

  return QColor(r, g, b);
}

ParticleSystem::ParticleSystem() : m_effectFinished(false) {}

void ParticleSystem::update(float deltaTimeInSeconds) {
  bool effectFinished = true;
  for (Particle &particle : m_particles) {
    particle.update(deltaTimeInSeconds);
    if (!particle.isDead())
      effectFinished = false;
  }

  m_effectFinished = effectFinished;
}

ParticleSystem::~ParticleSystem() {}

void ParticleSystem::spawnParticles(int count, QColor color,
                                    float lifespanInSeconds) {
  bool randomColor = color == nullptr;
  for (int i = 0; i < count; i++) {
    if (randomColor)
      color = getRandomColor();
    Particle particle(m_position, lifespanInSeconds, color);
    m_particles.push_back(particle);
  }
}

bool ParticleSystem::effectFinished() const {
  if (m_particles.empty())
    return true;
  return m_effectFinished;
}

void ParticleSystem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)

  if (!m_particles.empty()) {
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->save();
    painter->setPen(Qt::NoPen);
    for (Particle &particle : m_particles) {
      particle.draw(*painter);
    }
    painter->restore();
  }
}

void ParticleSystem::setPosition(QPointF newPosition) {
  for (Particle &p : m_particles) {
    p.setPosition(newPosition);
  }

  m_position = newPosition;
}

} // namespace Effects
} // namespace Graphics
