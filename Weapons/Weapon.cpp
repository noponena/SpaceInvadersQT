#include "Weapon.h"
#include "GameObjects/Ships/Ship.h"

namespace Weapons {

Weapon::Weapon()
    : m_owner(nullptr), m_soundEnabled(true), m_minCooldownMs(100),
      m_firstShot(false) {
  {
    clampCooldownMs();
    m_lastFiredTimer.start();
  }
}

void Weapon::fire() {
  if (canShoot()) {
    m_lastFiredTimer.restart();
    std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
        createProjectile();
    projectile->initialize();

    QPointF ownerCenter = m_owner->getGraphicsItem()->boundingRect().center();
    QPointF projectileCenter =
        projectile->getGraphicsItem()->boundingRect().center();
    QPointF delta = projectileCenter - ownerCenter;
    float newX = projectile->getPosition().x() - delta.x();
    float newY = projectile->getPosition().y() - delta.y();
    GameObjects::Position projectilePosition = projectile->getPosition();
    projectilePosition.setPos(QPointF(newX, newY));
    projectile->setPosition(projectilePosition);
    emit projectileFired(std::move(projectile));
  }
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

std::unique_ptr<GameObjects::Projectiles::Projectile>
Weapon::createProjectile() {
  auto projectile = m_projectilePrototype->clone();
  projectile->setPosition(m_owner->getPosition());
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

bool Weapon::canShoot() {
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
