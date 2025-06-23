#include "GameHUD.h"
#include "Graphics/PixmapLibrary.h"

namespace Game {
namespace Core {

GameHUD::GameHUD(int width, int height)
    : m_previousPlayerEnergy(0), m_previousPlayerHealth(0) {
  m_background = new QGraphicsRectItem(0, 0, width, height * 0.1, this);
  qDebug() << "initializing game hud with width" << width << "and height"
           << height;
  m_background->setBrush(QColor(22, 21, 19));
  m_energyBar = new Graphics::EnergyBar(0, width / 2, 20, true, this);
  m_healthBar = new Graphics::HealthBar(0, width / 2, 20, true, this);
  QPixmap weapon1;
  QPixmap weapon2;
  QPixmap weapon3;
  QPixmap weapon4;

  Graphics::CooldownItem *item1 = new Graphics::CooldownItem(weapon1, this);
  m_weaponCooldownItems[0] = item1;

  Graphics::CooldownItem *item2 = new Graphics::CooldownItem(weapon2, this);
  m_weaponCooldownItems[1] = item2;

  Graphics::CooldownItem *item3 = new Graphics::CooldownItem(weapon3, this);
  m_weaponCooldownItems[2] = item3;

  Graphics::CooldownItem *item4 = new Graphics::CooldownItem(weapon4, this);
  m_weaponCooldownItems[3] = item4;
  for (int i = 0; i < 4; ++i) {
    m_weaponCooldownItems[i]->setPos(i * 100 + width / 2 - 200, 15);
  }

  m_healthBar->setPos(width / 4, 80);
  m_energyBar->setPos(width / 4, 100);

  this->setZValue(100);
}

void GameHUD::onPlayerSecondaryWeaponsChanged(
    std::unique_ptr<Weapons::Weapon> weapons[]) {
  for (std::uint32_t i = 0; i < 4; i++) {
    QString path;
    if (weapons[i]) {
      path = weapons[i]->getHudImagePath();
    } else {
      path = ":/Images/weapon_slot.png";
    }
    QPixmap pixmap = Graphics::PixmapLibrary::getPixmap(path, 50, 50);
    m_weaponCooldownItems[i]->setPixmap(pixmap);
  }
}

void GameHUD::onPlayerSecondaryWeaponFired(std::uint32_t weaponIndex,
                                           std::uint32_t cooldownMs) {
  m_weaponCooldownItems[weaponIndex]->startCooldown(cooldownMs);
}

void GameHUD::onPlayerEnergyUpdated(float updatedEnergy) {
  if (m_previousPlayerEnergy == updatedEnergy)
    return;
  m_previousPlayerEnergy = updatedEnergy;
  m_energyBar->setProgress(updatedEnergy);
}

void GameHUD::onPlayerMaxEnergySet(float maxEnergy) {
  m_energyBar->setMaxProgress(maxEnergy);
}

void GameHUD::onPlayerHealthUpdated(float updatedHealth) {
  if (m_previousPlayerHealth == updatedHealth)
    return;
  m_previousPlayerHealth = updatedHealth;
  m_healthBar->setProgress(updatedHealth);
}

void GameHUD::onPlayerMaxHealthSet(float maxHealth) {
  m_healthBar->setMaxProgress(maxHealth);
}

} // namespace Core
} // namespace Game
