#include "WeaponBuilder.h"

namespace Weapons {

WeaponBuilder::WeaponBuilder()
    : m_weapon(nullptr), m_cooldownMs(0) {}

WeaponBuilder WeaponBuilder::clone() const
{
    WeaponBuilder builder;
    if (m_weapon)
        builder.m_weapon = this->m_weapon->clone();
    else
        logNullPointerWarning();
    return builder;
}

WeaponBuilder &WeaponBuilder::createWeapon(std::unique_ptr<Weapon> weapon)
{
    m_weapon = std::move(weapon);
    return *this;
}

WeaponBuilder &WeaponBuilder::withSound(bool soundEnabled)
{
    if (m_weapon) {
        if (soundEnabled)
            m_weapon->enableSound();
        else
            m_weapon->disableSound();
    }
    else
        logNullPointerWarning();
    return *this;
}

WeaponBuilder &WeaponBuilder::withWeaponCooldownMs(const int cooldownMs)
{
    if (m_weapon)
        m_weapon->setCooldownMs(cooldownMs);
    else
        logNullPointerWarning();
    return *this;
}

WeaponBuilder &WeaponBuilder::withProjectile(std::unique_ptr<GameObjects::Projectiles::Projectile> projectile)
{
    if (m_weapon)
        m_weapon->setProjectilePrototype(std::move(projectile));
    else
        logNullPointerWarning();
    return *this;
}

std::unique_ptr<Weapon> WeaponBuilder::build()
{
    return m_weapon->clone();
}

void WeaponBuilder::logNullPointerWarning() const
{
    qCritical() << "createWeapon must be called before setting any weapon properties.";
}



}
