#include "Ship.h"

namespace GameObjects {

Ship::Ship(const int maxHp, float speed, const Position &position)
    : GameObject(position, speed), m_maxHp(maxHp), m_speed(speed)
{
    m_currentHp = maxHp;
}

void Ship::takeDamage(int amount)
{
    m_currentHp -= amount;
    if (m_currentHp < 0) {
        m_currentHp = 0;
    }
}

void Ship::heal(int amount)
{
    m_currentHp += amount;
    if (m_currentHp > m_maxHp) {
        m_currentHp = m_maxHp;
    }
}

bool Ship::isAlive()
{
    return m_currentHp > 0;
}


} // namespace GameObjects
