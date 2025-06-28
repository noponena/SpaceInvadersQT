#include "ShipWithHealthBar.h"

namespace GameObjects {
namespace Ships {

ShipWithHealthBar::ShipWithHealthBar(const Config::GameContext &ctx)
    : Ship(ctx) {}

void ShipWithHealthBar::initialize() {
  m_currentHealth = m_maxHealth;
  GameObject::initialize();
  m_healthBar =
      std::make_unique<Graphics::HealthBar>(m_currentHealth, 50, 5, false);
  m_healthBar->setPos(0, 45); // Position it below the ship
}

void ShipWithHealthBar::takeDamage(std::uint32_t amount) {
  Ship::takeDamage(amount);
  if (!m_immortal)
    m_healthBar->updateProgress(-static_cast<float>(amount));
}

void ShipWithHealthBar::heal(std::uint32_t amount) {
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
