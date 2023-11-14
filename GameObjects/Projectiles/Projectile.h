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

class Projectile : public GameObject {
public:
  Projectile();
  Projectile(int damage = 1,
             std::unordered_set<Weapons::ProjectileProperty> properties = {});
  virtual ~Projectile() = default;
  virtual std::unique_ptr<Projectile> clone() const = 0;

  bool shouldBeDeleted() override;
  void collideWith(GameObject &other) override;
  void collideWithEnemyShip(Ships::EnemyShip &enemyShip) override;
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override;
  int getDamage() const;
  void setDamage(int newDamage);

  void setProperties(
      const std::unordered_set<Weapons::ProjectileProperty> &newProperties);
  void addProperty(const Weapons::ProjectileProperty property);
  void removeProperty(const Weapons::ProjectileProperty property);

protected:
  int m_damage;
  std::unordered_set<Weapons::ProjectileProperty> m_properties;
};
} // namespace Projectiles
} // namespace GameObjects

#endif // PROJECTILE_H
