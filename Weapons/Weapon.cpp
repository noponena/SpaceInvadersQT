#include "Weapon.h"

Weapons::Weapon::Weapon(float cooldownMs,
                        float minCooldownMs,
                        Game::Movement::MovementStrategy movementStrategy)
    : m_owner(nullptr), m_cooldownMs(cooldownMs), m_minCooldownMs(minCooldownMs),
    m_movementStrategy(movementStrategy)
{
    {
      this->clampCooldownMs();
      m_lastShotTimer.start();
    }
}

void Weapons::Weapon::shoot()
{
    if (this->canShoot()) {
      m_lastShotTimer.restart();
      GameObjects::Projectiles::Projectile *projectile =
          this->createProjectile();
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
      projectile->setMovementStrategy(m_movementStrategy);
      emit projectileShot(projectile);
    }
}

void Weapons::Weapon::setOwner(GameObjects::Ships::Ship *newOwner)
{
    m_owner = newOwner;
}

void Weapons::Weapon::updateWeaponCooldown(float amount)
{
    m_cooldownMs += amount;
    clampCooldownMs();
}

bool Weapons::Weapon::canShoot()
{
    int elapsed = m_lastShotTimer.elapsed();
    if (elapsed < m_cooldownMs) {
      return false;
    }

    return true;
}

void Weapons::Weapon::clampCooldownMs()
{
    if (m_cooldownMs < m_minCooldownMs)
      m_cooldownMs = m_minCooldownMs;
}
