#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjects/GameObject.h"
#include <QUrl>
#include <set>

namespace Weapons {
enum class ProjectileProperty;
}

namespace GameObjects {
namespace Projectiles {

enum class ProjectileProperty { PIERCING, HOMING };

class Projectile : public GameObject {
public:
  Projectile(const Config::GameContext &ctx);
  virtual ~Projectile() = default;
  virtual std::unique_ptr<GameObject> clone() const override;

  bool shouldBeDeleted() override;
  void collideWith(GameObject &other) override;
  void collideWithEnemyShip(Ships::EnemyShip &enemyShip) override;
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override;
  int getDamage() const;
  void setDamage(int newDamage);

  void
  setProperties(const std::unordered_set<ProjectileProperty> &newProperties);
  void addProperty(const ProjectileProperty property);
  void removeProperty(const ProjectileProperty property);

  QString hudPixmapResourcePath() const;

protected:
  int m_damage;
  QString m_hudPixmapResourcePath = "";
  std::unordered_set<ProjectileProperty> m_properties;
  void initializeObjectType() override;
  void initializeSounds() override {};
};
} // namespace Projectiles
} // namespace GameObjects

#endif // PROJECTILE_H
