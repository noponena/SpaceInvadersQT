#include "GLWeaponBar.h"
#include "Graphics/TextureRegistry.h"
#include <algorithm>

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

static const char* texturedVertSrc = R"(
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoord;
uniform vec2 uOffset;
uniform vec2 uScale;
out vec2 vTexCoord;
void main() {
    gl_Position = vec4(position * uScale + uOffset, 0.0, 1.0);
    vTexCoord = texcoord;
}
)";

static const char* texturedFragSrc = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 fragColor;
uniform sampler2D uTexture;
uniform float uAlpha;
void main() {
    vec4 texColor = texture(uTexture, vTexCoord);
    fragColor = vec4(texColor.rgb, texColor.a * uAlpha);
}
)";

static const char* cooldownTexturedFragSrc = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 fragColor;
uniform float uCooldown; // 1 = full, 0 = empty
uniform vec4 uColor;

void main() {
    // vTexCoord in [0,1] range for quad covering entire slot
    // Calculate angle relative to (0.5,0.5) center, y axis up
    float dx = vTexCoord.x - 0.5;
    float dy = 0.5 - vTexCoord.y;
    float angle = atan(dy, dx); // radians
    if (angle < 0.0)
        angle += 2.0 * 3.14159265;
    float pct = angle / (2.0 * 3.14159265);
    pct = mod(pct - 0.25, 1.0);

    // Keep all fragments, just mask based on pct and uCooldown
    if (pct > uCooldown)
        discard;

    fragColor = uColor;
}
)";

GLWeaponBar::GLWeaponBar(int slotCount, float slotSize, UISizeMode sizeMode)
    : m_slotCount(slotCount),
    m_slotSize(slotSize),
    m_sizeMode(sizeMode),
    m_posX(0.5f),
    m_posY(0.05f),
    m_posMode(UISizeMode::Fraction),
    m_barBorderColor(0.7f, 0.7f, 0.7f, 1.0f),
    m_slotBorderColor(0.2f, 0.2f, 0.2f, 1.0f),
    m_slotBackgroundColor(0.13f, 0.13f, 0.17f, 1.0f)
{}

GLWeaponBar::~GLWeaponBar() {
    if (m_program)
        delete m_program;
    if (m_textureProgram) delete m_textureProgram;
    if (m_cooldownPieProgram) delete m_cooldownPieProgram;
}

void GLWeaponBar::destroyGL(QOpenGLFunctions_3_3_Core *gl) {
    if (m_vao) gl->glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) gl->glDeleteBuffers(1, &m_vbo);
    if (m_texVao) gl->glDeleteVertexArrays(1, &m_texVao);
    if (m_texVbo) gl->glDeleteBuffers(1, &m_texVbo);
}

void GLWeaponBar::setSlotCount(int count) {
    m_slotCount = std::max(1, count);
    m_slots.resize(m_slotCount);
}

void GLWeaponBar::setSlotSize(float size, UISizeMode mode) {
    m_slotSize = size;
    m_sizeMode = mode;
}

void GLWeaponBar::setPosition(float x, float y, UISizeMode mode) {
    m_posX = x;
    m_posY = y;
    m_posMode = mode;
}

void GLWeaponBar::startCooldown(int slotIdx, float cooldownSeconds, float currentTimeSec)
{
    if (slotIdx < 0 || slotIdx >= m_slotCount) return;
    m_slots[slotIdx].cooldownDurationSec = cooldownSeconds;
    m_slots[slotIdx].cooldownStartTime = currentTimeSec;
    m_slots[slotIdx].isCoolingDown = true;
}

void GLWeaponBar::setBarBorderColor(const QVector4D& color) { m_barBorderColor = color; }
void GLWeaponBar::setSlotBorderColor(const QVector4D& color) { m_slotBorderColor = color; }
void GLWeaponBar::setSlotBackgroundColor(const QVector4D& color) { m_slotBackgroundColor = color; }

void GLWeaponBar::setSlotImage(int index, const QString &imagePath)
{
    if (index < 0 || index >= m_slotCount) return;
    m_slots[index].imagePath = imagePath;

    if (!imagePath.isEmpty()) {
        const auto& texInfo = Graphics::TextureRegistry::instance().getOrCreateTexture(imagePath);
        m_slots[index].texture = texInfo.handle;
    } else {
        m_slots[index].texture = 0;
    }
}

void GLWeaponBar::clearSlotImages()
{
    for (auto& slot : m_slots) {
        slot.imagePath.clear();
        slot.texture = 0;
        slot.isCoolingDown = false;
        slot.cooldownDurationSec = 0;
    }
}
void GLWeaponBar::setBorderThickness(float px) { m_borderThickness = px; }

void GLWeaponBar::initialize(QOpenGLFunctions_3_3_Core* gl) {
    if (m_initialized) return;

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
    gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
    gl->glBindVertexArray(0);

    if (!m_textureProgram) {
        m_textureProgram = new QOpenGLShaderProgram();
        m_textureProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, texturedVertSrc);
        m_textureProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, texturedFragSrc);
        m_textureProgram->link();

        float quadVerts[] = {
            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
        };

        gl->glGenVertexArrays(1, &m_texVao);
        gl->glGenBuffers(1, &m_texVbo);
        gl->glBindVertexArray(m_texVao);
        gl->glBindBuffer(GL_ARRAY_BUFFER, m_texVbo);
        gl->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);
        gl->glEnableVertexAttribArray(0); // position
        gl->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        gl->glEnableVertexAttribArray(1); // texcoord
        gl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
        gl->glBindVertexArray(0);
    }

    if (!m_cooldownPieProgram) {
        m_cooldownPieProgram = new QOpenGLShaderProgram();
        m_cooldownPieProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, texturedVertSrc); // same as icon shader
        m_cooldownPieProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, cooldownTexturedFragSrc);
        m_cooldownPieProgram->link();
    }

    m_initialized = true;
}

void GLWeaponBar::render(QOpenGLFunctions_3_3_Core* gl, float screenWidth, float screenHeight, float currentGameTime) {
    if (!m_initialized)
        initialize(gl);

    // Calculate slot size in pixels
    float slotPx = (m_sizeMode == UISizeMode::Fraction) ? m_slotSize * screenWidth : m_slotSize;
    float barW = slotPx * m_slotCount;
    float barH = slotPx;

    // Bar origin
    float x = (m_posMode == UISizeMode::Fraction) ? m_posX * screenWidth : m_posX;
    float y = (m_posMode == UISizeMode::Fraction) ? m_posY * screenHeight : m_posY;

    // Center horizontally at given X/Y (anchor = bar center)
    x -= barW / 2.0f;
    y -= barH / 2.0f;

    gl->glEnable(GL_BLEND);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_program->bind();

    // --- Draw outer bar border ---
    float outerBorder = m_borderThickness;
    QVector2D uScale_bar((barW + 2 * outerBorder) * 2.0f / screenWidth,
                         (barH + 2 * outerBorder) * 2.0f / screenHeight);
    QVector2D uOffset_bar(((x - outerBorder) * 2.0f / screenWidth) - 1.0f,
                          1.0f - ((y - outerBorder) * 2.0f / screenHeight) - uScale_bar.y());
    m_program->setUniformValue("uScale", uScale_bar);
    m_program->setUniformValue("uOffset", uOffset_bar);
    m_program->setUniformValue("uColor", m_barBorderColor);

    gl->glBindVertexArray(m_vao);
    gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    gl->glBindVertexArray(0);

    // --- Draw each slot (background + border) ---
    for (int i = 0; i < m_slotCount; ++i) {
        float slotX = x + i * slotPx;
        float slotY = y;

        // Slot border
        float sBorder = m_borderThickness;
        QVector2D uScale_slotBorder((slotPx + 2 * sBorder) * 2.0f / screenWidth,
                                    (slotPx + 2 * sBorder) * 2.0f / screenHeight);
        QVector2D uOffset_slotBorder(((slotX - sBorder) * 2.0f / screenWidth) - 1.0f,
                                     1.0f - ((slotY - sBorder) * 2.0f / screenHeight) - uScale_slotBorder.y());
        m_program->setUniformValue("uScale", uScale_slotBorder);
        m_program->setUniformValue("uOffset", uOffset_slotBorder);
        m_program->setUniformValue("uColor", m_slotBorderColor);

        gl->glBindVertexArray(m_vao);
        gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        gl->glBindVertexArray(0);

        // Slot background
        QVector2D uScale_slot(slotPx * 2.0f / screenWidth,
                              slotPx * 2.0f / screenHeight);
        QVector2D uOffset_slot((slotX * 2.0f / screenWidth) - 1.0f,
                               1.0f - (slotY * 2.0f / screenHeight) - uScale_slot.y());
        m_program->setUniformValue("uScale", uScale_slot);
        m_program->setUniformValue("uOffset", uOffset_slot);
        m_program->setUniformValue("uColor", m_slotBackgroundColor);

        gl->glBindVertexArray(m_vao);
        gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        gl->glBindVertexArray(0);
    }
    m_program->release();
    m_textureProgram->bind();
    for (int i = 0; i < m_slotCount; ++i) {
        GLuint tex = m_slots[i].texture;
        if (tex) {
            float slotX = x + i * slotPx;
            float slotY = y;
            float iconMargin = 2.0f;
            float iconW = slotPx - 2 * iconMargin;
            float iconH = slotPx - 2 * iconMargin;
            float iconX = slotX + iconMargin;
            float iconY = slotY + iconMargin;
            drawTexturedQuad(gl, tex, iconX, iconY, iconW, iconH, screenWidth, screenHeight, 1.0f);
        }
    }
    m_textureProgram->release();
    m_cooldownPieProgram->bind();
    for (int i = 0; i < m_slotCount; ++i) {
        float slotX = x + i * slotPx;
        float slotY = y;
        WeaponSlot& slot = m_slots[i];
        if (!slot.isCoolingDown) continue;

        float elapsed = currentGameTime - slot.cooldownStartTime;
        float t = std::clamp(1.0f - elapsed / slot.cooldownDurationSec, 0.0f, 1.0f);

        if (t <= 0.0f) {
            slot.isCoolingDown = false;
            continue; // no overlay, cooldown ended
        }

        // Draw pie overlay with t (fraction remaining)
        drawCooldownPieOverlay(gl, slotX, slotY, slotPx, slotPx, t, screenWidth, screenHeight);
    }
    m_cooldownPieProgram->release();
}

void GLWeaponBar::drawTexturedQuad(
    QOpenGLFunctions_3_3_Core* gl,
    GLuint texture,
    float x, float y,
    float w, float h,
    float screenW, float screenH,
    float alpha
    ) {

    QVector2D uScale(w * 2.0f / screenW, h * 2.0f / screenH);
    QVector2D uOffset((x * 2.0f / screenW) - 1.0f, 1.0f - (y * 2.0f / screenH) - uScale.y());

    gl->glBindVertexArray(m_texVao);

    m_textureProgram->setUniformValue("uScale", uScale);
    m_textureProgram->setUniformValue("uOffset", uOffset);
    m_textureProgram->setUniformValue("uAlpha", alpha);

    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, texture);

    gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    gl->glBindVertexArray(0);
}

void GLWeaponBar::drawCooldownPieOverlay(
    QOpenGLFunctions_3_3_Core* gl,
    float x, float y,
    float w, float h,
    float cooldownFraction, // [0,1]
    float screenW, float screenH
    ) {

    QVector2D uScale(w * 2.0f / screenW, h * 2.0f / screenH);
    QVector2D uOffset((x * 2.0f / screenW) - 1.0f, 1.0f - (y * 2.0f / screenH) - uScale.y());

    m_cooldownPieProgram->setUniformValue("uScale", uScale);
    m_cooldownPieProgram->setUniformValue("uOffset", uOffset);
    m_cooldownPieProgram->setUniformValue("uCooldown", cooldownFraction);
    m_cooldownPieProgram->setUniformValue("uColor", QVector4D(0.3,0.3,0.3,0.6)); // semi-transparent grey

    gl->glBindVertexArray(m_texVao);
    gl->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    gl->glBindVertexArray(0);
}


}
