#include "Projectile.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/Weapon.h"

namespace GameObjects {
namespace Projectiles {

Projectile::Projectile()
    : GameObject(Position(0, 0)), m_damage(1), m_properties({}) {}

Projectile::Projectile(
    int damage, std::unordered_set<Weapons::ProjectileProperty> properties)
    : GameObject(Position(0, 0)), m_damage(damage), m_properties(properties) {}

bool Projectile::shouldBeDeleted() {
  bool isPiercing = m_properties.find(Weapons::ProjectileProperty::PIERCING) !=
                    m_properties.end();
  return GameObject::shouldBeDeleted() || (!isPiercing && m_hasCollided);
}

void Projectile::collideWith(GameObject &other) {
  other.collideWithProjectile(*this);
}

void Projectile::collideWithEnemyShip(Ships::EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  m_hasCollided = true;
}

void Projectile::collideWithPlayerShip(Ships::PlayerShip &playerShip) {
  Q_UNUSED(playerShip);
  m_hasCollided = true;
}

int Projectile::getDamage() const { return m_damage; }

void Projectile::setDamage(int newDamage) { m_damage = newDamage; }

void Projectile::setProperties(
    const std::unordered_set<Weapons::ProjectileProperty> &newProperties) {
  m_properties = newProperties;
}

void Projectile::addProperty(const Weapons::ProjectileProperty property) {
  m_properties.insert(property);
}

void Projectile::removeProperty(const Weapons::ProjectileProperty property) {
  m_properties.erase(property);
}

} // namespace Projectiles

} // namespace GameObjects
