#pragma once

#include "UI/UISizeMode.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector4D>

namespace UI {

struct WeaponSlot {
    QString imagePath;
    GLuint texture = 0;
    float cooldownDurationSec = 0.0f;
    float cooldownStartTime = 0.0f;
    bool isCoolingDown = false;
};

class GLWeaponBar {
public:
    GLWeaponBar(int slotCount = 4, float slotSize = 0.07f, UISizeMode sizeMode = UISizeMode::Fraction);
    ~GLWeaponBar();

    void setSlotCount(int count);
    void setSlotSize(float size, UISizeMode mode = UISizeMode::Fraction);
    void setPosition(float x, float y, UISizeMode mode = UISizeMode::Fraction);

    void startCooldown(int slotIdx, float cooldownSeconds, float currentTimeSec);

    void setBarBorderColor(const QVector4D& color);
    void setSlotBorderColor(const QVector4D& color);
    void setSlotBackgroundColor(const QVector4D& color);

    void setSlotImage(int index, const QString& imagePath);
    void clearSlotImages();

    void setBorderThickness(float px);

    // Main render call
    void render(QOpenGLFunctions_3_3_Core *gl,
                float screenWidth,
                float screenHeight,
                float currentGameTime);

    void destroyGL(QOpenGLFunctions_3_3_Core *gl);

private:
    void initialize(QOpenGLFunctions_3_3_Core* gl);
    bool m_initialized = false;
    std::vector<WeaponSlot> m_slots;

    int m_slotCount;
    float m_slotSize;
    UISizeMode m_sizeMode;

    float m_posX, m_posY;
    UISizeMode m_posMode;

    QVector4D m_barBorderColor;
    QVector4D m_slotBorderColor;
    QVector4D m_slotBackgroundColor;
    float m_borderThickness = 2.0f; // px (screen pixel units)

    // OpenGL
    QOpenGLShaderProgram* m_program = nullptr;
    QOpenGLShaderProgram* m_textureProgram = nullptr;
    QOpenGLShaderProgram* m_cooldownPieProgram = nullptr;
    GLuint m_vao = 0, m_vbo = 0;
    GLuint m_texVao = 0, m_texVbo = 0;
    void drawTexturedQuad(QOpenGLFunctions_3_3_Core *gl,
                          GLuint texture,
                          float x,
                          float y,
                          float w,
                          float h,
                          float screenW,
                          float screenH,
                          float alpha);
    void drawCooldownPieOverlay(
        QOpenGLFunctions_3_3_Core *gl, float x, float y, float w, float h, float cooldownFraction, float screenW, float screenH);
};
}
