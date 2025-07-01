#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QVector3D>

class GLBar {
public:
  GLBar(float maxValue, const QVector2D &pos, const QVector2D &size,
        const QVector3D &color);
  ~GLBar();

  void setValue(float value);
  void setMaxValue(float maxValue);
  void render(QOpenGLFunctions_3_3_Core *gl);

private:
  void initialize(QOpenGLFunctions_3_3_Core *gl);

  float m_value;
  float m_maxValue;
  QVector2D m_pos;
  QVector2D m_size;
  QVector3D m_color;

  GLuint m_vao = 0;
  GLuint m_vbo = 0;
  QOpenGLShaderProgram *m_program = nullptr;
  bool m_initialized = false;
};
