#include "Projectile.h"
#include "GameObjects/Ships/EnemyShip.h"

namespace GameObjects {
namespace Projectiles {

Projectile::Projectile(Position pos, float speed, bool hostile, int damage)
    : GameObject(pos, speed), m_hostile(hostile), m_damage(damage) {}

bool Projectile::shouldBeDeleted() {
  return m_position.isBeyondScreenTopLimit(10); //|| m_hasCollided;
}

void Projectile::collideWith(GameObject &other) {
  other.collideWithProjectile(*this);
}

void Projectile::collideWithEnemyShip(Ships::EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  m_hasCollided = true;
}

int Projectile::getDamage() const { return m_damage; }
bool Projectile::hostile() const { return m_hostile; }

} // namespace Projectiles

} // namespace GameObjects
