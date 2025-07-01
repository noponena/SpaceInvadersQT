#include "GameHUD.h"
#include <GL/gl.h>

GameHUD::GameHUD(int width, int height, QObject *parent)
    : QObject(parent), m_width(width), m_height(height),
      m_energyBar(100.0f, QVector2D(width / 4, height - 80),
                  QVector2D(width / 2, 20), QVector3D(0.2f, 0.5f, 1.0f)),
      m_healthBar(100.0f, QVector2D(width / 4, height - 110),
                  QVector2D(width / 2, 20), QVector3D(0.9f, 0.2f, 0.2f)) {
  // Setup initial positions, etc.
  for (int i = 0; i < 4; ++i) {
    // m_weaponCooldownItems[i].setPosition(width / 2 - 200 + i * 100, height -
    // 40);
  }
}

void GameHUD::render(QOpenGLFunctions_3_3_Core *gl) {
  // Set up orthographic projection for 2D HUD
  gl->glDisable(GL_DEPTH_TEST);
  // ...set projection matrix to ortho(0, m_width, m_height, 0)
  // Render background panel here if you want (simple GL quad)
  // Render health/energy bars and cooldowns:
  m_healthBar.render(gl);
  m_energyBar.render(gl);
  // for (auto& item : m_weaponCooldownItems) item.render();
  gl->glEnable(GL_DEPTH_TEST);
}

// Slots:
void GameHUD::onPlayerSecondaryWeaponsChanged(
    const std::array<QString, 4> &weaponTexturePaths) {
  // for (int i = 0; i < 4; ++i)
  // m_weaponCooldownItems[i].setTexture(weaponTexturePaths[i].toStdString());
}
void GameHUD::onPlayerSecondaryWeaponFired(std::uint32_t weaponIndex,
                                           std::uint32_t cooldownMs) {
  // if (weaponIndex < m_weaponCooldownItems.size())
  // m_weaponCooldownItems[weaponIndex].startCooldown(cooldownMs);
}
void GameHUD::onPlayerEnergyUpdated(float updatedEnergy) {
  if (m_previousPlayerEnergy != updatedEnergy) {
    m_previousPlayerEnergy = updatedEnergy;
    m_energyBar.setValue(updatedEnergy);
  }
}
void GameHUD::onPlayerMaxEnergySet(float maxEnergy) {
  m_energyBar.setMaxValue(maxEnergy);
}
void GameHUD::onPlayerHealthUpdated(float updatedHealth) {
  if (m_previousPlayerHealth != updatedHealth) {
    m_previousPlayerHealth = updatedHealth;
    m_healthBar.setValue(updatedHealth);
  }
}
void GameHUD::onPlayerMaxHealthSet(float maxHealth) {
  m_healthBar.setMaxValue(maxHealth);
}
