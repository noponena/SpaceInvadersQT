#ifndef GAMEOBJECTS_SHIPS_SHIPWITHHEALTHBAR_H
#define GAMEOBJECTS_SHIPS_SHIPWITHHEALTHBAR_H

#include "Graphics/HealthBar.h"
#include "Ship.h"

namespace GameObjects {
namespace Ships {

class ShipWithHealthBar : public Ship {
public:
  ShipWithHealthBar(const std::uint32_t maxHp, const float speed,
                    const Position &position);
  void initialize() override;

protected:
  std::unique_ptr<Graphics::HealthBar> m_healthBar;

  // Ship interface
public:
  void takeDamage(std::uint32_t amount) override;
  void heal(std::uint32_t amount) override;
  void kill() override;
  void restoreHealth() override;
};

} // namespace Ships
} // namespace GameObjects

#endif // GAMEOBJECTS_SHIPS_SHIPWITHHEALTHBAR_H
