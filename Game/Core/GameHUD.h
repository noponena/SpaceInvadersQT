#ifndef GAME_CORE_GAMEHUD_H
#define GAME_CORE_GAMEHUD_H

#include "Graphics/CooldownItem.h"
#include "Graphics/EnergyBar.h"
#include "Graphics/HealthBar.h"
#include "Weapons/Weapon.h"
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>

namespace Game {
namespace Core {

class GameHUD : public QObject, public QGraphicsItemGroup {
  Q_OBJECT
public:
  GameHUD(int width, int height);

public slots:
  void
  onPlayerSecondaryWeaponsChanged(std::unique_ptr<Weapons::Weapon> weapons[]);
  void onPlayerSecondaryWeaponFired(unsigned weaponIndex, unsigned cooldownMs);
  void onPlayerEnergyUpdated(float updatedEnergy);
  void onPlayerMaxEnergySet(float maxEnergy);
  void onPlayerHealthUpdated(float updatedHealth);
  void onPlayerMaxHealthSet(float maxHealth);

private:
  Graphics::CooldownItem *m_weaponCooldownItems[4];
  Graphics::EnergyBar *m_energyBar;
  Graphics::HealthBar *m_healthBar;
  float m_previousPlayerEnergy;
  float m_previousPlayerHealth;
  QGraphicsRectItem *m_background;
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_GAMEHUD_H
