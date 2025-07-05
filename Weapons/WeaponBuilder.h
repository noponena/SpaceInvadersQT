#ifndef WEAPONS_WEAPONFACTORY_H
#define WEAPONS_WEAPONFACTORY_H

#include "Weapon.h"
#include <memory>

namespace Weapons {

class WeaponBuilder {
public:
    WeaponBuilder();
    WeaponBuilder clone() const;
    template <typename WeaponType> WeaponBuilder &createWeapon() {
        m_weapon = std::make_unique<WeaponType>();
        return *this;
    }
    WeaponBuilder &withSound(bool soundEnabled);
    WeaponBuilder &withWeaponCooldownMs(const std::uint32_t cooldownMs);
    WeaponBuilder &withEnergyConsuption(const std::uint32_t energyConsuption);
    WeaponBuilder &withProjectile(
        std::unique_ptr<GameObjects::Projectiles::Projectile> projectile);
    std::unique_ptr<Weapon> build();

private:
    std::unique_ptr<Weapon> m_weapon;
    std::unique_ptr<GameObjects::Projectiles::Projectile> m_projectile;
    int m_cooldownMs;

    void logNullPointerWarning() const;
};

} // namespace Weapons

#endif // WEAPONS_WEAPONFACTORY_H
