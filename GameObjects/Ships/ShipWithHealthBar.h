#ifndef GAMEOBJECTS_SHIPS_SHIPWITHHEALTHBAR_H
#define GAMEOBJECTS_SHIPS_SHIPWITHHEALTHBAR_H

#include "HealthBar.h"
#include "Ship.h"

namespace GameObjects {
namespace Ships {

class ShipWithHealthBar : public Ship {
public:
  ShipWithHealthBar(const int maxHp, const float speed,
                    const Position &position);
  void initialize() override;

protected:
  std::unique_ptr<HealthBar> m_healthBar;

  // Ship interface
public:
  void takeDamage(int amount) override;
  void heal(int amount) override;
};

} // namespace Ships
} // namespace GameObjects

#endif // GAMEOBJECTS_SHIPS_SHIPWITHHEALTHBAR_H
