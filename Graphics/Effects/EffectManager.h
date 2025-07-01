#pragma once

#include <vector>
#include <memory>
#include <QVector2D>
#include <QColor>
#include <QOpenGLShaderProgram>
#include "ParticleSystem.h"

namespace Graphics {
namespace Effects {


class EffectManager {
public:
    static EffectManager& instance() {
        static EffectManager s_instance;
        return s_instance;
    }

    EffectManager(const EffectManager&) = delete;
    EffectManager& operator=(const EffectManager&) = delete;

    ~EffectManager();

    void initializeGL(QOpenGLFunctions_3_3_Core* glFuncs);
    void render(QOpenGLFunctions_3_3_Core* glFuncs, const QVector2D& viewport);

    void update(float deltaTime);
    void render(const QVector2D& viewport);
    void spawnDestructionEffect(const QVector2D &pos,
                                float lifeSpan = 0.8f,
                                int count = 25,
                                const std::optional<QColor> color = nullptr);
    void clear();

private:
    EffectManager();
    QOpenGLShaderProgram *m_particleProgram = nullptr;
    QOpenGLFunctions_3_3_Core* m_glFuncs = nullptr;
    std::vector<std::unique_ptr<ParticleSystem>> m_effects;
    void destroyGL();
};

} // namespace Effects
} // namespace Graphics
