#include "ShipWithHealthBar.h"

namespace GameObjects {
namespace Ships {

ShipWithHealthBar::ShipWithHealthBar(const unsigned int maxHp,
                                     const float speed,
                                     const Position &position)
    : Ship(maxHp, speed, position) {}

void ShipWithHealthBar::initialize() {
  GameObject::initialize();
  m_healthBar = std::make_unique<Graphics::HealthBar>(m_currentHealth, 50, 5,
                                                      m_graphicsItem.get());
  m_healthBar->setPos(0, 45); // Position it below the ship
}

void ShipWithHealthBar::takeDamage(unsigned int amount) {
  Ship::takeDamage(amount);
  if (!m_immortal)
    m_healthBar->updateProgress(-static_cast<float>(amount));
}

void ShipWithHealthBar::heal(unsigned int amount) {
  Ship::heal(amount);
  m_healthBar->updateProgress(amount);
}

void ShipWithHealthBar::kill() {
  Ship::kill();
  m_healthBar->updateProgress(-9999999);
}

void ShipWithHealthBar::restoreHealth() {
  Ship::restoreHealth();
  m_healthBar->updateProgress(9999999);
}

} // namespace Ships
} // namespace GameObjects
