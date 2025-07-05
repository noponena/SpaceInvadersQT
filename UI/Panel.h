#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector4D>

namespace UI {
class Panel {
public:
    Panel(const QVector4D& color = QVector4D(0.10f, 0.10f, 0.10f, 0.7f), float heightFraction = 0.12f);
    ~Panel();

    void setColor(const QVector4D& color);
    void setHeightFraction(float fraction);
    void render(QOpenGLFunctions_3_3_Core* gl, float screenWidth, float screenHeight);

private:
    void initialize(QOpenGLFunctions_3_3_Core* gl);

    QVector4D m_color;
    float m_heightFraction;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    QOpenGLShaderProgram* m_program = nullptr;
    bool m_initialized = false;
};
}


