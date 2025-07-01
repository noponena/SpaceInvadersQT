#include "GLBar.h"

static const char *VERT_SHADER = R"(
#version 330
layout(location = 0) in vec2 position;
uniform vec2 uOffset;
uniform vec2 uScale;
void main() {
    gl_Position = vec4((position * uScale) + uOffset, 0.0, 1.0);
}
)";

static const char *FRAG_SHADER = R"(
#version 330
uniform vec3 uColor;
out vec4 fragColor;
void main() {
    fragColor = vec4(uColor, 1.0);
}
)";

GLBar::GLBar(float maxValue, const QVector2D &pos, const QVector2D &size,
             const QVector3D &color)
    : m_value(maxValue), m_maxValue(maxValue), m_pos(pos), m_size(size),
      m_color(color) {}

GLBar::~GLBar() {
  if (m_program)
    delete m_program;
  // Clean up VAO, VBO if needed...
}

void GLBar::setValue(float value) {
  m_value = std::max(0.0f, std::min(value, m_maxValue));
}

void GLBar::setMaxValue(float maxValue) {
  m_maxValue = maxValue;
  if (m_value > m_maxValue)
    m_value = m_maxValue;
}

void GLBar::initialize(QOpenGLFunctions_3_3_Core *gl) {
  if (m_initialized)
    return;
  m_program = new QOpenGLShaderProgram();
  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, VERT_SHADER);
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, FRAG_SHADER);
  m_program->link();

  float verts[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
  gl->glGenVertexArrays(1, &m_vao);
  gl->glGenBuffers(1, &m_vbo);

  gl->glBindVertexArray(m_vao);
  gl->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  gl->glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                            (void *)0);
  gl->glEnableVertexAttribArray(0);

  gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
  gl->glBindVertexArray(0);

  m_initialized = true;
}

void GLBar::render(QOpenGLFunctions_3_3_Core *gl) {
  if (!m_initialized)
    initialize(gl);

  // Calculate the width to render based on progress
  float percent = m_maxValue > 0 ? (m_value / m_maxValue) : 0.f;
  QVector2D drawSize(m_size.x() * percent, m_size.y());

  m_program->bind();

  // Normalized device coords: offset/scale in [-1, 1]
  QVector2D uScale(drawSize.x() * 2.0f / 800.0f,
                   drawSize.y() * 2.0f /
                       600.0f); // Change 800/600 to your viewport!
  QVector2D uOffset((m_pos.x() * 2.0f / 800.0f) - 1.0f,
                    1.0f - (m_pos.y() * 2.0f / 600.0f) - uScale.y());

  m_program->setUniformValue("uOffset", uOffset);
  m_program->setUniformValue("uScale", uScale);
  m_program->setUniformValue("uColor", m_color);

  gl->glBindVertexArray(m_vao);
  gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  gl->glBindVertexArray(0);

  m_program->release();
}
