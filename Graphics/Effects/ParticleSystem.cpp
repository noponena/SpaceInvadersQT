#include "ParticleSystem.h"
#include "Utils/Utils.h"

namespace Graphics {
namespace Effects {

ParticleSystem::ParticleSystem(int maxParticles)
    : m_maxParticles(maxParticles), m_vao(0), m_vbo(0)
{}

void ParticleSystem::initializeGL(QOpenGLFunctions_3_3_Core* glFuncs) {
    glFuncs->glGenVertexArrays(1, &m_vao);
    glFuncs->glGenBuffers(1, &m_vbo);

    glFuncs->glBindVertexArray(m_vao);
    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glFuncs->glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glFuncs->glEnableVertexAttribArray(0);
    glFuncs->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glFuncs->glEnableVertexAttribArray(1);
    glFuncs->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, 0);
    glFuncs->glBindVertexArray(0);
}


void ParticleSystem::destroyGL(QOpenGLFunctions_3_3_Core *glFuncs) {
    if (m_vao) glFuncs->glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glFuncs->glDeleteBuffers(1, &m_vbo);
    m_vao = 0;
    m_vbo = 0;
}

void ParticleSystem::uploadToGPU(QOpenGLFunctions_3_3_Core* glFuncs) {
    m_vertexData.clear();
    /*
    for (const Particle& p : m_particles) {
        if (!p.isAlive()) continue;
        float normAlpha = std::clamp(p.life / p.lifeSpan, 0.0f, 1.0f);
        QColor c = p.color;
        m_vertexData.push_back(p.position.x());
        m_vertexData.push_back(p.position.y());
        m_vertexData.push_back(c.redF());
        m_vertexData.push_back(c.greenF());
        m_vertexData.push_back(c.blueF());
        m_vertexData.push_back(normAlpha * c.alphaF());

        // qDebug() << "[ParticleSystem] r" << c.redF() << "g" << c.greenF() << "b" << c.blueF() << "a" << (normAlpha * c.alphaF());
    }
*/
    for (int i = 0; i < 100; ++i) {
        m_vertexData.push_back(i * 20.0f);  // X: 0, 20, 40, ...
        m_vertexData.push_back(200.0f);     // Y: all at 200
        m_vertexData.push_back(1.0f);       // R
        m_vertexData.push_back(0.0f);       // G
        m_vertexData.push_back(1.0f);       // B
        m_vertexData.push_back(1.0f);       // A
    }
    qDebug() << "[ParticleSystem] Vertex count:" << m_vertexData.size() / 6;
    if (!m_particles.empty())
        qDebug() << "[ParticleSystem] First particle pos:" << m_particles[0].position << "life:" << m_particles[0].life;
    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glFuncs->glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexData.size() * sizeof(float), m_vertexData.data());
    glFuncs->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::render(QOpenGLFunctions_3_3_Core* glFuncs)
{
    glFuncs->glEnable(GL_PROGRAM_POINT_SIZE);
    glFuncs->glDisable(GL_DEPTH_TEST);
    glFuncs->glDisable(GL_BLEND);

    uploadToGPU(glFuncs);

    glFuncs->glBindVertexArray(m_vao);
    glFuncs->glDrawArrays(GL_POINTS, 0, m_vertexData.size() / 6);
    glFuncs->glBindVertexArray(0);

    GLenum err = glFuncs->glGetError();
    if (err != GL_NO_ERROR)
        qCritical() << "[ParticleSystem] OpenGL error:" << err;
}

bool ParticleSystem::allParticlesDead() const
{
    return m_allParticlesDead;
}

void ParticleSystem::update(float dt) {
    bool anyAlive = false;
    for (Particle& p : m_particles) {
        if (p.isAlive())
        {
            p.update(dt);
            anyAlive = true;
        }
    }
    m_allParticlesDead = !anyAlive;
}

void ParticleSystem::spawnParticles(int count,
                                    const QVector2D &pos,
                                    float lifeSpan,
                                    float maxSpeed,
                                    const std::optional<QColor> color)
{
    int spawned = 0;
    m_allParticlesDead = false;
    if (color.has_value()) {
        qDebug() << "[ParticleSystem] Spawning" << color.value() << "partciles with lifeSpan" << lifeSpan << "and maxSpeed" << maxSpeed;
    } else {
        qDebug() << "[ParticleSystem] Spawning random colored partciles with lifeSpan" << lifeSpan << "and maxSpeed" << maxSpeed;
    }
    for (Particle& p : m_particles) {
        if (!p.isAlive() && spawned < count) {
            QColor pColor = color.value_or(Utils::getRandomColor());
            p = Particle(pos, lifeSpan, pColor, maxSpeed);
            ++spawned;
        }
    }
    while (spawned < count && (int)m_particles.size() < m_maxParticles) {
        QColor pColor = color.value_or(Utils::getRandomColor());
        m_particles.emplace_back(pos, lifeSpan, pColor, maxSpeed);
        ++spawned;
    }

    qDebug() << "[ParticleSystem] Spawned" << spawned << "particles to position" << pos;
}

} // namespace Effects
} // namespace Graphics
