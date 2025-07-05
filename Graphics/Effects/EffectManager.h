#pragma once

#include "ParticleSystem.h"
#include <QColor>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <memory>
#include <vector>

namespace Graphics {
namespace Effects {

class EffectManager {
public:
  static EffectManager &instance() {
    static EffectManager s_instance;
    return s_instance;
  }

  EffectManager(const EffectManager &) = delete;
  EffectManager &operator=(const EffectManager &) = delete;

  ~EffectManager();

  void initializeGL(QOpenGLFunctions_3_3_Core *glFuncs);
  void render(QOpenGLFunctions_3_3_Core *glFuncs, const QVector2D &viewport);

  void update(float deltaTime);
  void render(const QVector2D &viewport);
  void spawnDestructionEffect(const QVector2D &pos, float lifeSpan = 0.8f,
                              int maxSpeed = 250, int count = 25,
                              const std::optional<QColor> color = std::nullopt);
  void clear();

private:
  EffectManager();
  QOpenGLShaderProgram *m_particleProgram = nullptr;
  QOpenGLFunctions_3_3_Core *m_glFuncs = nullptr;
  std::vector<std::unique_ptr<ParticleSystem>> m_effects;
  std::vector<std::unique_ptr<ParticleSystem>> m_pendingInit;
  void destroyGL();
};

} // namespace Effects
} // namespace Graphics
