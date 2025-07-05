#pragma once

#include "UI/IRenderable.h"
#include <QVector4D>
#include <qopenglshaderprogram.h>

namespace UI {

enum class UISizeMode { Pixels, Fraction };

class GLProgressBar : public IRenderable {
public:
  GLProgressBar(float minValue = 0.f, float maxValue = 100.f,
                float width = 0.07f, float height = 0.05f,
                UISizeMode sizeMode = UISizeMode::Fraction);

  ~GLProgressBar();

  void setValue(float value);
  void setRange(float minValue, float maxValue);

  // Bar color transitions: full, mid, low, plus thresholds (default: .6, .3)
  void setBarColors(const QVector4D &full, const QVector4D &mid,
                    const QVector4D &low);
  void setThresholds(float mid, float low);
  void setBackgroundColor(const QVector4D &color);

  void render(QOpenGLFunctions_3_3_Core *gl, float screenWidth,
              float screenHeight, float x = 0.f, float y = 0.f);

  void setCenter(float x, float y, UISizeMode mode = UISizeMode::Pixels);

private:
  void initialize(QOpenGLFunctions_3_3_Core *gl);

  float m_minValue;
  float m_maxValue;
  float m_value;

  float m_width;
  float m_height;

  float m_centerX = 0.f;
  float m_centerY = 0.f;
  bool m_useCenter = false;

  float m_borderThickness = 1.0f;
  QVector4D m_borderColor = QVector4D(0.98f, 0.96f, 0.92f, 1.0f);

  UISizeMode m_sizeMode;
  UISizeMode m_uiPosMode;

  QVector4D m_bgColor;
  QVector4D m_fullColor;
  QVector4D m_midColor;
  QVector4D m_lowColor;

  float m_midThreshold;
  float m_lowThreshold;

  GLuint m_vao = 0;
  GLuint m_vbo = 0;
  QOpenGLShaderProgram *m_program = nullptr;
  bool m_initialized = false;
};
} // namespace UI
