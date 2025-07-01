#ifndef GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
#define GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H

#include "Particle.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <optional>

namespace Graphics {
namespace Effects {

class ParticleSystem {
public:
  explicit ParticleSystem(int maxParticles = 10000);

  void spawnParticles(int count, const QVector2D &pos, float lifeSpan,
                      float maxSpeed, const std::optional<QColor> color);
  void update(float deltaTime);
  void render(QOpenGLFunctions_3_3_Core *glFuncs);
  bool allParticlesDead() const;
  void initializeGL(QOpenGLFunctions_3_3_Core *glFuncs);
  void destroyGL(QOpenGLFunctions_3_3_Core *glFuncs);

  ~ParticleSystem() = default;

private:
  std::vector<Particle> m_particles;
  bool m_allParticlesDead;

  // OpenGL resources:
  GLuint m_vbo = 0;
  GLuint m_vao = 0;
  int m_maxParticles = 0;

  // Internal buffer for sending data to GPU
  std::vector<float> m_vertexData; // [x, y, r, g, b, a]
  void uploadToGPU(QOpenGLFunctions_3_3_Core *glFuncs);
};

} // namespace Effects
} // namespace Graphics

#endif // GAMEOBJECTS_EFFECTS_PARTICLESYSTEM_H
