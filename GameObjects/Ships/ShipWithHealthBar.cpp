#include "ShipWithHealthBar.h"

namespace GameObjects {
namespace Ships {

ShipWithHealthBar::ShipWithHealthBar(const int maxHp, const float speed,
                                     const Position &position)
    : Ship(maxHp, speed, position) {}

void ShipWithHealthBar::initialize() {
  GameObject::initialize();
  m_healthBar =
      std::make_unique<HealthBar>(m_currentHp, 50, 5, m_graphicsItem.get());
  m_healthBar->setPos(0, 45); // Position it below the ship
}

void ShipWithHealthBar::takeDamage(int amount) {
  Ship::takeDamage(amount);
  m_healthBar->updateHealth(-amount);
}

void ShipWithHealthBar::heal(int amount) {
  Ship::heal(amount);
  m_healthBar->updateHealth(amount);
}

} // namespace Ships
} // namespace GameObjects
