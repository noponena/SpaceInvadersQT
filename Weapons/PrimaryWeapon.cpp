#include "PrimaryWeapon.h"

namespace Weapons {
std::unique_ptr<Weapon> PrimaryWeapon::clone() const {
  std::unique_ptr<PrimaryWeapon> weapon = std::make_unique<PrimaryWeapon>();
  weapon->m_owner = m_owner;
  weapon->setEnergyConsuption(energyConsuption());
  weapon->m_projectilePrototype =
      std::unique_ptr<GameObjects::Projectiles::Projectile>(
          std::unique_ptr<GameObjects::Projectiles::Projectile>(
              static_cast<GameObjects::Projectiles::Projectile *>(
                  m_projectilePrototype->clone().release())));
  weapon->m_soundEnabled = m_soundEnabled;
  weapon->m_cooldownMs = m_cooldownMs;
  return weapon;
}

QString PrimaryWeapon::getHudImagePath() const {
  // No hud images for primary weapons at this point.
  return "";
}
} // namespace Weapons
