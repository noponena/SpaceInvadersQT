#include "EffectManager.h"
#include <windows.h>
#include <GL/gl.h>


namespace Graphics {
namespace Effects {

const char *vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 vertPos;
layout(location = 1) in vec4 vertColor;

uniform vec2 viewport;

out vec4 fragColor;

void main() {
    vec2 ndc = (vertPos / viewport) * 2.0 - 1.0;
    ndc.y = -ndc.y; // if Qt Y-up, else omit
    gl_Position = vec4(ndc, 0, 1);
    gl_PointSize = 48.0; // Or make this a uniform/attribute for size fade
    fragColor = vertColor;
}
)";

const char *fragmentShaderSrc = R"(
#version 330 core
out vec4 color;
void main() {
    color = vec4(1,0,1,1);
}
)";
EffectManager::EffectManager() {
    m_particleProgram = new QOpenGLShaderProgram;
}

EffectManager::~EffectManager() {
    destroyGL();
    if (m_particleProgram) {
        delete m_particleProgram;
        m_particleProgram = nullptr;
    }
}

void EffectManager::initializeGL(QOpenGLFunctions_3_3_Core *glFuncs)
{
    m_glFuncs = glFuncs;

    // Initialize particle shader program
    if (!m_particleProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc))
        qCritical() << "Failed to load vertex shader:" << m_particleProgram->log();

    if (!m_particleProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc))
        qCritical() << "Failed to load fragment shader:" << m_particleProgram->log();

    if (!m_particleProgram->link())
        qCritical() << "Failed to link shader program:" << m_particleProgram->log();

    if (!m_particleProgram->isLinked())
        qCritical() << "Particle shader program not linked successfully!";
}

void EffectManager::update(float deltaTime)
{
    if (!m_glFuncs || m_effects.empty()) return;
    qDebug() << "[EffectManager] Updating effects..";
    auto it = m_effects.begin();
    while (it != m_effects.end()) {
        (*it)->update(deltaTime);
        if ((*it)->allParticlesDead()) {
            (*it)->destroyGL(m_glFuncs);
            it = m_effects.erase(it);
            qDebug() << "[EffectManager] ParticleSystem deleted";
        } else {
            ++it;
        }
    }
}

void EffectManager::render(const QVector2D& viewport) {
    if (!m_glFuncs || m_effects.empty()) return;
    qDebug() << "[EffectManager] Rendering effects.. viewport:" << viewport;
    m_particleProgram->bind();
    m_particleProgram->setUniformValue("viewport", viewport);

    for (auto& effect : m_effects)
        effect->render(m_glFuncs);

    m_particleProgram->release();
}

void EffectManager::spawnDestructionEffect(const QVector2D& pos, float lifeSpan, int count, const std::optional<QColor> color) {
    auto system = std::make_unique<ParticleSystem>(128);
    system->initializeGL(m_glFuncs);
    system->spawnParticles(count, pos, lifeSpan, 50.0f, color);
    m_effects.push_back(std::move(system));
    qDebug() << "[EffectManager] ParticleSystem added";
}

void EffectManager::clear() {
    destroyGL();
    m_effects.clear();
}

void EffectManager::destroyGL() {
    if (!m_glFuncs) return;
    for (auto& effect : m_effects)
        effect->destroyGL(m_glFuncs);
    m_glFuncs = nullptr;
}

} // namespace Effects
} // namespace Graphics
