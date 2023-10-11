#include "Shooter.h"

namespace GameObjects {

Shooter::Shooter(int maxHp, float speed, int fireRate, Position position)
    : Ship(maxHp, speed, position), m_fireRate(fireRate)
{
    m_shotCooldown = 1000 / m_fireRate;
}

void Shooter::updateFireRate(int amount)
{
    m_fireRate += amount;
    if (m_fireRate < 1)
    {
        m_fireRate = 1;
    } else if (m_fireRate > 1000) {
        m_fireRate = 1000;
    }
    m_shotCooldown = 1000 / m_fireRate;
}

int Shooter::fireRate() const
{
    return m_fireRate;
}

bool Shooter::canShoot()
{
    int elapsed = m_lastShotTime.elapsed();  // Time in milliseconds since last shot
    if (elapsed < m_shotCooldown) {
        return false;
    }

    m_lastShotTime.restart();
    return true;
}

} // namespace GameObjects
