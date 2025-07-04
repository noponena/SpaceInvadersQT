#include "SecondaryWeapon.h"

namespace Weapons {

std::unique_ptr<Weapon> SecondaryWeapon::clone() const {
  std::unique_ptr<SecondaryWeapon> weapon = std::make_unique<SecondaryWeapon>();
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

QString SecondaryWeapon::getHudImagePath() const {
  return m_projectilePrototype->getHudPixmapPath();
}

} // namespace Weapons
