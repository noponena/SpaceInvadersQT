#ifndef WEAPONS_WEAPONFACTORY_H
#define WEAPONS_WEAPONFACTORY_H

#include "Weapon.h"
#include <memory>


namespace Weapons {

class WeaponBuilder {
public:
    WeaponBuilder()
        : m_weapon(nullptr),
          m_movementStrategy(Game::Movement::StationaryMovementStrategy()),
          m_projectileProperties({}),
          m_projectileDamage(1),
          m_cooldownMs(0),
          m_projectileHostility(false) {}

    WeaponBuilder& createWeapon(std::unique_ptr<Weapon> weapon) {
        m_weapon = std::move(weapon);
        return *this;
    }

    WeaponBuilder& cloneWeapon(const std::unique_ptr<Weapon>& weapon) {
        if (weapon) {
            m_weapon = std::unique_ptr<Weapon>(weapon->clone());
        } else {
            m_weapon.reset();
        }
        return *this;
    }

    WeaponBuilder& withSound(bool soundEnabled) {
        if (soundEnabled) m_weapon->enableSound();
        else m_weapon->disableSound();
        return *this;
    }

    WeaponBuilder& withWeaponCooldownMs(const int cooldownMs) {
        m_cooldownMs = cooldownMs;
        return *this;
    }

    WeaponBuilder& withProjectileDamage(const int damage) {
        m_projectileDamage = damage;
        return *this;
    }

    WeaponBuilder& withProjectile(GameObjects::Projectiles::Projectile *projectile) {
        m_projectile = projectile;
        return *this;
    }

    WeaponBuilder& withProjectileMovementStrategy(const Game::Movement::MovementStrategy& strategy) {
        m_movementStrategy = strategy;
        return *this;
    }

    WeaponBuilder& withProjectileProperty(const ProjectileProperty property) {
        m_projectileProperties.insert(property);
        return *this;
    }

    WeaponBuilder& withProjectileHostility(const bool hostility) {
        m_projectileHostility = hostility;
        return *this;
    }

    std::unique_ptr<Weapon> build() {
        std::unique_ptr<GameObjects::Projectiles::Projectile> projectileClone = m_projectile->clone();
        projectileClone->setHostile(m_projectileHostility);
        projectileClone->setDamage(m_projectileDamage);
        projectileClone->setMovementStrategy(m_movementStrategy);
        projectileClone->setProperties(m_projectileProperties);
        m_weapon->setProjectilePrototype(std::move(projectileClone));
        m_weapon->setCooldownMs(m_cooldownMs);
        return std::move(m_weapon);
    }


private:
    std::unique_ptr<Weapon> m_weapon;
    Game::Movement::MovementStrategy m_movementStrategy = Game::Movement::StationaryMovementStrategy();
    std::set<ProjectileProperty> m_projectileProperties = {};
    GameObjects::Projectiles::Projectile *m_projectile;
    int m_projectileDamage;
    int m_cooldownMs;
    bool m_projectileHostility;
};

} // namespace Weapons

#endif // WEAPONS_WEAPONFACTORY_H



