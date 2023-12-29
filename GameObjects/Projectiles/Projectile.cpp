#include "Projectile.h"
#include "GameObjects/Ships/EnemyShip.h"

namespace GameObjects {
namespace Projectiles {

Projectile::Projectile()
    : GameObject(Position(0, 0)), m_damage(1), m_properties({}) {}

std::unique_ptr<GameObject> Projectile::clone() const {
  std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>();
  projectile->m_objectTypes = m_objectTypes;
  projectile->m_magnetism = m_magnetism;
  projectile->setSpawnSoundInfo(m_spawnSoundInfo);
  projectile->setDestructionSoundInfo(m_destructionSoundInfo);
  projectile->setDamage(m_damage);
  projectile->setProperties(m_properties);
  projectile->setPixmapData(m_pixmapData);
  projectile->setMovementStrategy(movementStrategy());
  return projectile;
}

bool Projectile::shouldBeDeleted() {
  bool isPiercing =
      m_properties.find(ProjectileProperty::PIERCING) != m_properties.end();

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
    const std::unordered_set<ProjectileProperty> &newProperties) {
  m_properties = newProperties;
}

void Projectile::addProperty(const ProjectileProperty property) {
  m_properties.insert(property);
}

void Projectile::removeProperty(const ProjectileProperty property) {
  m_properties.erase(property);
}

void Projectile::initializeObjectType() {
  m_objectTypes.insert(ObjectType::PROJECTILE);
}

} // namespace Projectiles

} // namespace GameObjects
