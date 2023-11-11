#include "Projectile.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/Weapon.h"

namespace GameObjects {
namespace Projectiles {

Projectile::Projectile(Position pos, float speed, bool hostile, int damage, std::set<Weapons::WeaponProperty> properties)
    : GameObject(pos, speed), m_hostile(hostile), m_damage(damage), m_properties(properties) {}

bool Projectile::shouldBeDeleted() {
    bool isPiercing = m_properties.find(Weapons::WeaponProperty::PIERCING) != m_properties.end();
    return m_position.isBeyondAnyLimit(50) || (!isPiercing && m_hasCollided);
}

void Projectile::collideWith(GameObject &other) {
  other.collideWithProjectile(*this);
}

void Projectile::collideWithEnemyShip(Ships::EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  if (!m_hostile)
      m_hasCollided = true;
}

void Projectile::collideWithPlayerShip(Ships::PlayerShip &playerShip)
{
  Q_UNUSED(playerShip);
  if (m_hostile)
      m_hasCollided = true;
}

int Projectile::getDamage() const { return m_damage; }
bool Projectile::hostile() const { return m_hostile; }

} // namespace Projectiles

} // namespace GameObjects
