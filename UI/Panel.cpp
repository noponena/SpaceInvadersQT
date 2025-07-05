#include "Panel.h"

namespace UI {

static const char* vertSrc = R"(
#version 330 core
layout(location = 0) in vec2 position;
uniform vec2 uOffset;
uniform vec2 uScale;
void main() {
    gl_Position = vec4(position * uScale + uOffset, 0.0, 1.0);
}
)";

static const char* fragSrc = R"(
#version 330 core
uniform vec4 uColor;
out vec4 fragColor;
void main() {
    fragColor = uColor;
}
)";

Panel::Panel(const QVector4D& color, float heightFraction)
    : m_color(color), m_heightFraction(heightFraction)
{}

Panel::~Panel() {
    if (m_program) delete m_program;
    // Optionally delete VAO/VBO if needed (for small apps, can skip)
}

void Panel::setColor(const QVector4D& color) {
    m_color = color;
}

void Panel::setHeightFraction(float fraction) {
    m_heightFraction = qBound(0.0f, fraction, 1.0f);
}

void Panel::initialize(QOpenGLFunctions_3_3_Core* gl) {
    if (m_initialized) return;

    // Setup program
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertSrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragSrc);
    m_program->link();

    // Setup rectangle (in [0,1] local panel space)
    float verts[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    gl->glGenVertexArrays(1, &m_vao);
    gl->glGenBuffers(1, &m_vbo);

    gl->glBindVertexArray(m_vao);
    gl->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl->glBindVertexArray(0);

    m_initialized = true;
}

void Panel::render(QOpenGLFunctions_3_3_Core* gl, float screenWidth, float screenHeight) {
    if (!m_initialized) initialize(gl);

    // Determine panel size (bottom of the screen, full width)
    float panelHeight = screenHeight * m_heightFraction;
    float panelWidth = screenWidth;
    float panelX = 0.0f;
    float panelY = screenHeight - panelHeight;

    QVector2D uScale(panelWidth * 2.0f / screenWidth, panelHeight * 2.0f / screenHeight);
    QVector2D uOffset(
        (panelX * 2.0f / screenWidth) - 1.0f,
        1.0f - (panelY * 2.0f / screenHeight) - uScale.y()
    );

    // Enable blending for transparency
    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_program->bind();
    m_program->setUniformValue("uOffset", uOffset);
    m_program->setUniformValue("uScale", uScale);
    m_program->setUniformValue("uColor", m_color);

    gl->glBindVertexArray(m_vao);
    gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    gl->glBindVertexArray(0);

    m_program->release();
}
}
