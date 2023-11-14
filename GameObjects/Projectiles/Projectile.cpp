#include "Projectile.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/Weapon.h"

namespace GameObjects {
namespace Projectiles {

Projectile::Projectile()
    : GameObject(Position(0, 0)), m_hostile(false),
    m_damage(1), m_properties({})
{
    m_objectType = ObjectType::PROJECTILE;
}

Projectile::Projectile(bool hostile, int damage, std::set<Weapons::ProjectileProperty> properties)
    : GameObject(Position(0, 0)), m_hostile(hostile), m_damage(damage), m_properties(properties)
{
    m_objectType = ObjectType::PROJECTILE;
}

bool Projectile::shouldBeDeleted() {
    bool isPiercing = m_properties.find(Weapons::ProjectileProperty::PIERCING) != m_properties.end();
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

void Projectile::setHostile(bool newHostile)
{
  m_hostile = newHostile;
}

void Projectile::setDamage(int newDamage)
{
  m_damage = newDamage;
}

void Projectile::setProperties(const std::set<Weapons::ProjectileProperty> &newProperties)
{
  m_properties = newProperties;
}

void Projectile::addProperty(const Weapons::ProjectileProperty property)
{
  m_properties.insert(property);
}

void Projectile::removeProperty(const Weapons::ProjectileProperty property)
{
  m_properties.erase(property);
}

} // namespace Projectiles

} // namespace GameObjects
