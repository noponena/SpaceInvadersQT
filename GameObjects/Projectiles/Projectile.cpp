#include "Projectile.h"
#include "GameObjects/Ships/EnemyShip.h"

namespace GameObjects
{
Projectile::Projectile(Position pos, float speed, bool hostile, int damage)
    : GameObject(pos, speed), m_hostile(hostile), m_damage(damage)
{

}

bool Projectile::shouldBeDeleted()
{
    return m_position.isBeyondScreenTopLimit() || m_hasCollided;
}

void Projectile::collideWith(GameObject &other)
{
    other.collideWithProjectile(*this);
    qDebug() << "projectile collided!";
}

void Projectile::collideWithEnemyShip(EnemyShip &enemyShip)
{
    m_hasCollided = true;
    enemyShip.collideWithProjectile(*this);
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

    //this->graphicsItem()->setPos(this->m_position.getX(), this->m_position.getY());
