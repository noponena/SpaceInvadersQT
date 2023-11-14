#include "Weapon.h"

namespace Weapons {

Weapon::Weapon()
    : m_owner(nullptr),
    m_soundEnabled(true),
    m_minCooldownMs(100)
{
    {
        this->clampCooldownMs();
        m_lastShotTimer.start();
    }
}

void Weapon::shoot()
{
    if (this->canShoot()) {
        m_lastShotTimer.restart();
        std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
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
        emit projectileShot(std::move(projectile));
    }
}

void Weapon::setOwner(GameObjects::Ships::Ship *newOwner)
{
    m_owner = newOwner;
}

void Weapon::setProjectilePrototype(std::unique_ptr<GameObjects::Projectiles::Projectile> prototype)
{
    m_projectilePrototype = std::move(prototype);
}

void Weapon::setCooldownMs(float newCooldownMs)
{
    m_cooldownMs = newCooldownMs;
}

void Weapon::updateWeaponCooldown(float amount)
{
    m_cooldownMs += amount;
    clampCooldownMs();
}

void Weapon::enableSound()
{
    m_soundEnabled = true;
}

void Weapon::disableSound()
{
    m_soundEnabled = false;
}

void Weapon::addProjectileProperty(ProjectileProperty property)
{
    m_projectilePrototype->addProperty(property);
}

void Weapon::removeProjectileProperty(ProjectileProperty property)
{
    m_projectilePrototype->removeProperty(property);
}

bool Weapon::canShoot()
{
    if (!m_owner->isDead()) {
        int elapsed = m_lastShotTimer.elapsed();
        if (elapsed >= m_cooldownMs) {
            return true;
        }
    }
    return false;
}

void Weapon::clampCooldownMs()
{
    if (m_cooldownMs < m_minCooldownMs)
        m_cooldownMs = m_minCooldownMs;
}

}
