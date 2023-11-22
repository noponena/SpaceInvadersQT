#include "ParticleSystem.h"
#include <QTimer>
#include <thread>
#include <chrono>


namespace Graphics {
namespace Effects {

QColor ParticleSystem::getRandomColor() {
  int r = QRandomGenerator::global()->bounded(256);
  int g = QRandomGenerator::global()->bounded(256);
  int b = QRandomGenerator::global()->bounded(256);

  return QColor(r, g, b);
}

ParticleSystem::ParticleSystem() : m_effectFinished(false),
    m_particlesUpdated(false), m_particlesDrawn(true), m_updateThreadStarted(false)
{
  moveToThread(&m_updateThread);
  connect(&m_updateThread, &QThread::started, this, &ParticleSystem::runUpdateLoop);
}

void ParticleSystem::update(float deltaTimeInSeconds) {
  bool effectFinished = true;
  for (Particle &particle : m_particles) {
    particle.update(deltaTimeInSeconds);
    if (!particle.isDead())
      effectFinished = false;
  }

  m_effectFinished = effectFinished;
}

ParticleSystem::~ParticleSystem() {
  m_updateThread.quit();
  m_updateThread.wait();
}

void ParticleSystem::runUpdateLoop() {
  while (!m_effectFinished) {
    // Calculate deltaTime
    float deltaTimeInSeconds;
    if (!m_elapsedTimer.isValid()) {
      m_elapsedTimer.start();
      deltaTimeInSeconds = 0;
    } else {
      deltaTimeInSeconds = static_cast<float>(m_elapsedTimer.restart()) / 1000.0f;
    }

    // Update particles if they have been drawn
    update(deltaTimeInSeconds);
    m_particlesUpdated = true;
    m_particlesDrawn = false;
    // Sleep or wait for signal
    std::this_thread::sleep_for(std::chrono::milliseconds(8));
  }
}

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

bool ParticleSystem::effectFinished() const
{
  if (m_particles.empty())
    return true;
  return m_effectFinished;
}

void ParticleSystem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  Q_UNUSED(option)
  Q_UNUSED(widget)
  if (!m_updateThreadStarted) {
    m_updateThread.start();
    m_updateThreadStarted = true;
  }

  if (!m_particles.empty()) {
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->save();
    painter->setPen(Qt::NoPen);
    for (Particle &particle : m_particles) {
      particle.draw(*painter);
    }
    painter->restore();
    m_particlesDrawn = true;
    m_particlesUpdated = false;
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
