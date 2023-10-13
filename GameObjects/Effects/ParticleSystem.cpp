#include "ParticleSystem.h"
#include <QTimer>

namespace GameObjects {
namespace Effects {

ParticleSystem::ParticleSystem(QPointF position)
    : m_position(position)
{

}

QColor ParticleSystem::randomColor() {
    int r = QRandomGenerator::global()->bounded(256);
    int g = QRandomGenerator::global()->bounded(256);
    int b = QRandomGenerator::global()->bounded(256);

    return QColor(r, g, b);
}

void ParticleSystem::update()
{
    for (Particle &particle : m_particles) {
        particle.update();
    }

    m_particles.erase(std::remove_if(m_particles.begin(), m_particles.end(),
                      [](const Particle &p) { return p.isDead(); }), m_particles.end());
}

void ParticleSystem::spawnParticles(int count)
{
    for(int i = 0; i < count; i++) {
        Particle particle(m_position, 60, this->randomColor());
        m_particles.push_back(particle);
    }
}

void ParticleSystem::start()
{
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &ParticleSystem::update);
    updateTimer->start(16);
}

QRectF ParticleSystem::boundingRect() const {
    if (m_particles.empty()) {
        return QRectF();
    }
    qreal left = m_particles.front().position().x();
    qreal right = m_particles.front().position().x();
    qreal top = m_particles.front().position().y();
    qreal bottom = m_particles.front().position().y();

    for (const Particle &p : m_particles) {
        left = std::min(left, p.position().x());
        right = std::max(right, p.position().x());
        top = std::min(top, p.position().y());
        bottom = std::max(bottom, p.position().y());
    }

    return QRectF(left, top, right - left, bottom - top);
}


void ParticleSystem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    this->update();

    for (Particle &particle : m_particles) {
        particle.draw(*painter);
    }
}

} // namespace Effects
} // namespace GameObjects