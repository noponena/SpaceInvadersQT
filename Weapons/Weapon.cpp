#include "Weapon.h"
#include "GameObjects/Ships/Ship.h"
#include "Graphics/TextureRegistry.h"

namespace Weapons {

Weapon::Weapon()
    : m_owner(nullptr), m_soundEnabled(true), m_energyConsuption(0),
      m_minCooldownMs(100), m_firstShot(false) {
  {
    clampCooldownMs();
    m_lastFiredTimer.start();
  }
}

bool Weapon::fire() {
  if (!canFire())
    return false;

  m_lastFiredTimer.restart();
  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      createProjectile();
  projectile->initialize();

  emit projectileFired(std::move(projectile));
  return true;
}

void Weapon::setOwner(GameObjects::Ships::Ship *newOwner) {
  m_owner = newOwner;
}

void Weapon::setProjectilePrototype(
    std::unique_ptr<GameObjects::Projectiles::Projectile> prototype) {
  m_projectilePrototype = std::move(prototype);
}

void Weapon::setCooldownMs(float newCooldownMs) {
  m_cooldownMs = newCooldownMs;
  clampCooldownMs();
}

GameObjects::Projectiles::Projectile *Weapon::projectilePrototype() const {
  return m_projectilePrototype.get();
}

float Weapon::cooldownMs() const { return m_cooldownMs; }

std::uint32_t Weapon::energyConsuption() const { return m_energyConsuption; }

void Weapon::setEnergyConsuption(std::uint32_t newEnergyConsuption) {
  m_energyConsuption = newEnergyConsuption;
}

std::unique_ptr<GameObjects::Projectiles::Projectile>
Weapon::createProjectile() {
  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      std::unique_ptr<GameObjects::Projectiles::Projectile>(
          static_cast<GameObjects::Projectiles::Projectile *>(
              m_projectilePrototype->clone().release()));

  projectile->moveAbsolute(m_owner->getPosition());
  projectile->setSoundEnabled(m_soundEnabled);
  return projectile;
}

void Weapon::updateWeaponCooldown(float amount) {
  m_cooldownMs += amount;
  clampCooldownMs();
}

void Weapon::enableSound() { m_soundEnabled = true; }

void Weapon::disableSound() { m_soundEnabled = false; }

void Weapon::addProjectileProperty(
    GameObjects::Projectiles::ProjectileProperty property) {
  m_projectilePrototype->addProperty(property);
}

void Weapon::removeProjectileProperty(
    GameObjects::Projectiles::ProjectileProperty property) {
  m_projectilePrototype->removeProperty(property);
}

bool Weapon::canFire() {
  if (!m_owner->isDead()) {
    if (!m_firstShot) {
      m_firstShot = true;
      return true;
    }
    int elapsed = m_lastFiredTimer.elapsed();
    if (elapsed >= m_cooldownMs) {
      return true;
    }
  }
  return false;
}

void Weapon::clampCooldownMs() {
  if (m_cooldownMs < m_minCooldownMs)
    m_cooldownMs = m_minCooldownMs;
}

} // namespace Weapons
