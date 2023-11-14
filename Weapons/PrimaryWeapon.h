#ifndef WEAPONS_LASERCANNON_H
#define WEAPONS_LASERCANNON_H

#include "Weapon.h"

namespace Weapons {

class PrimaryWeapon : public Weapon {

  // Weapon interface
public:
  std::unique_ptr<GameObjects::Projectiles::Projectile>
  createProjectile() override {
    auto projectile = m_projectilePrototype->clone();
    projectile->setPosition(m_owner->getPosition());
    projectile->setSoundEnabled(m_soundEnabled);
    return projectile;
  }

  // Weapon interface
public:
  std::unique_ptr<Weapon> clone() const override {
    std::unique_ptr<PrimaryWeapon> weapon = std::make_unique<PrimaryWeapon>();
    weapon->m_owner = this->m_owner;
    weapon->m_projectilePrototype =
        std::unique_ptr<GameObjects::Projectiles::Projectile>(
            this->m_projectilePrototype->clone());
    weapon->m_soundEnabled = this->m_soundEnabled;
    weapon->m_cooldownMs = this->m_cooldownMs;
    return weapon;
  }
};

} // namespace Weapons

#endif // WEAPONS_LASERCANNON_H
