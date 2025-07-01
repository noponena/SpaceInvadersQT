#pragma once

#include "Graphics/GLBar.h"          // Custom OpenGL-based progress bar
#include "Graphics/GLCooldownItem.h" // Custom OpenGL-based cooldown widget
#include "Weapons/Weapon.h"
#include <QObject>
#include <array>

class GameHUD : public QObject {
  Q_OBJECT
public:
  explicit GameHUD(int width, int height, QObject *parent = nullptr);

  // Called from your QOpenGLWidget::paintGL() at the right time
  void render(QOpenGLFunctions_3_3_Core *gl);

public slots:
  void onPlayerSecondaryWeaponsChanged(const std::array<QString, 4> &);
  void onPlayerSecondaryWeaponFired(std::uint32_t weaponIndex,
                                    std::uint32_t cooldownMs);
  void onPlayerEnergyUpdated(float updatedEnergy);
  void onPlayerMaxEnergySet(float maxEnergy);
  void onPlayerHealthUpdated(float updatedHealth);
  void onPlayerMaxHealthSet(float maxHealth);

private:
  int m_width, m_height;
  // std::array<GLCooldownItem, 4> m_weaponCooldownItems;
  GLBar m_energyBar;
  GLBar m_healthBar;

  float m_previousPlayerEnergy = 0.0f;
  float m_previousPlayerHealth = 0.0f;
};
