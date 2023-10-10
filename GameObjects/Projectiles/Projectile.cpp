#include "Projectile.h"

namespace GameObjects
{
Projectile::Projectile(Position pos, float speed, bool hostile, int damage)
    : GameObject(pos, speed), m_hostile(hostile), m_damage(damage)
{

}

void Projectile::update(int deltaTime)
{
    this->moveUp(m_speed * deltaTime);
}

bool Projectile::shouldBeDeleted()
{
    return m_position.isAtScreenTopLimit() || m_hasCollided;
}

void Projectile::collideWith(GameObject &other)
{
    other.collideWithProjectile(*this);
    qDebug() << "projectile collided!";
}

void Projectile::collideWithEnemyShip(EnemyShip &enemyShip)
{
    m_hasCollided = true;
}

int Projectile::getDamage() const
{
    return m_damage;
}

bool Projectile::hostile() const
{
    return m_hostile;
}
}

