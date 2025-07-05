#include "GLProgressBar.h"
#include <QOpenGLShaderProgram>
#include <algorithm>

namespace UI {
static const char *vertSrc = R"(
#version 330 core
layout(location = 0) in vec2 position;
uniform vec2 uOffset;
uniform vec2 uScale;
void main() {
    gl_Position = vec4(position * uScale + uOffset, 0.0, 1.0);
}
)";

static const char *fragSrc = R"(
#version 330 core
uniform vec4 uColor;
out vec4 fragColor;
void main() {
    fragColor = uColor;
}
)";

GLProgressBar::GLProgressBar(float minValue, float maxValue, float width,
                             float height, UISizeMode sizeMode)
    : m_minValue(minValue), m_maxValue(maxValue), m_value(maxValue),
      m_width(width), m_height(height), m_sizeMode(sizeMode),
      m_bgColor(0.15f, 0.15f, 0.17f, 1.0f),
      m_fullColor(0.13f, 0.85f, 0.36f, 1.0f), // Green
      m_midColor(0.95f, 0.83f, 0.29f, 1.0f),  // Yellow
      m_lowColor(0.93f, 0.24f, 0.24f, 1.0f),  // Red
      m_midThreshold(0.6f), m_lowThreshold(0.3f) {}

GLProgressBar::~GLProgressBar() {
  if (m_program)
    delete m_program;
}

void GLProgressBar::initialize(QOpenGLFunctions_3_3_Core *gl) {
  if (m_initialized)
    return;

  m_program = new QOpenGLShaderProgram();
  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertSrc);
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragSrc);
  m_program->link();

  float verts[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

  gl->glGenVertexArrays(1, &m_vao);
  gl->glGenBuffers(1, &m_vbo);

  gl->glBindVertexArray(m_vao);
  gl->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  gl->glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  gl->glEnableVertexAttribArray(0);
  gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                            (void *)0);

  gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
  gl->glBindVertexArray(0);

  m_initialized = true;
}

void GLProgressBar::setValue(float value) {
  m_value = std::clamp(value, m_minValue, m_maxValue);
}

void GLProgressBar::setCenter(float x, float y, UISizeMode mode) {
  m_centerX = x;
  m_centerY = y;
  m_useCenter = true;
  m_uiPosMode = mode;
}

void GLProgressBar::destroyGL(QOpenGLFunctions_3_3_Core *gl) {
    if (m_vao) gl->glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) gl->glDeleteBuffers(1, &m_vbo);
}

void GLProgressBar::setRange(float minValue, float maxValue) {
  m_minValue = minValue;
  m_maxValue = maxValue;
  m_value = std::clamp(m_value, minValue, maxValue);
}

void GLProgressBar::setBarColors(const QVector4D &full, const QVector4D &mid,
                                 const QVector4D &low) {
  m_fullColor = full;
  m_midColor = mid;
  m_lowColor = low;
}

void GLProgressBar::setThresholds(float mid, float low) {
  m_midThreshold = mid;
  m_lowThreshold = low;
}

void GLProgressBar::setBackgroundColor(const QVector4D &color) {
  m_bgColor = color;
}

void GLProgressBar::render(QOpenGLFunctions_3_3_Core *gl, float screenWidth,
                           float screenHeight, float x, float y) {
  if (!m_initialized)
    initialize(gl);

  // Calculate size in pixels
  float w =
      (m_sizeMode == UISizeMode::Fraction) ? m_width * screenWidth : m_width;
  float h =
      (m_sizeMode == UISizeMode::Fraction) ? m_height * screenHeight : m_height;

  float xpos, ypos;

  if (m_useCenter) {
    if (m_uiPosMode == UISizeMode::Fraction) {
      xpos = m_centerX * screenWidth - w / 2.0f;
      ypos = m_centerY * screenHeight - h / 2.0f;
    } else {
      xpos = m_centerX - w / 2.0f;
      ypos = m_centerY - h / 2.0f;
    }
  } else {
    xpos = (m_sizeMode == UISizeMode::Fraction) ? x * screenWidth : x;
    ypos = (m_sizeMode == UISizeMode::Fraction) ? y * screenHeight : y;
  }

  // --- Draw border ---
  float bw = w + 2 * m_borderThickness;
  float bh = h + 2 * m_borderThickness;
  float border_x = xpos - m_borderThickness;
  float border_y = ypos - m_borderThickness;

  QVector2D uScale_border(bw * 2.0f / screenWidth, bh * 2.0f / screenHeight);
  QVector2D uOffset_border((border_x * 2.0f / screenWidth) - 1.0f,
                           1.0f - (border_y * 2.0f / screenHeight) -
                               uScale_border.y());

  // Value to percentage
  float percent = (m_maxValue > m_minValue)
                      ? (m_value - m_minValue) / (m_maxValue - m_minValue)
                      : 0.f;
  percent = std::clamp(percent, 0.f, 1.f);

  // -- Draw background bar --
  QVector2D uScale_bg(w * 2.0f / screenWidth, h * 2.0f / screenHeight);
  QVector2D uOffset_bg((xpos * 2.0f / screenWidth) - 1.0f,
                       1.0f - (ypos * 2.0f / screenHeight) - uScale_bg.y());

  gl->glDisable(GL_DEPTH_TEST);
  gl->glEnable(GL_BLEND);
  gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_program->bind();
  m_program->setUniformValue("uScale", uScale_border);
  m_program->setUniformValue("uOffset", uOffset_border);
  m_program->setUniformValue("uColor", m_borderColor);

  gl->glBindVertexArray(m_vao);
  gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  gl->glBindVertexArray(0);

  m_program->setUniformValue("uOffset", uOffset_bg);
  m_program->setUniformValue("uScale", uScale_bg);
  m_program->setUniformValue("uColor", m_bgColor);

  gl->glBindVertexArray(m_vao);
  gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  gl->glBindVertexArray(0);

  // -- Draw foreground (progress) bar --
  QVector2D uScale_fg(w * percent * 2.0f / screenWidth,
                      h * 2.0f / screenHeight);
  QVector2D uOffset_fg = uOffset_bg;

  // Pick color based on percent/threshold
  QVector4D barColor = m_fullColor;
  if (percent < m_lowThreshold)
    barColor = m_lowColor;
  else if (percent < m_midThreshold)
    barColor = m_midColor;

  m_program->setUniformValue("uScale", uScale_fg);
  m_program->setUniformValue("uOffset", uOffset_fg);
  m_program->setUniformValue("uColor", barColor);

  gl->glBindVertexArray(m_vao);
  gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  gl->glBindVertexArray(0);

  m_program->release();
}
} // namespace UI
