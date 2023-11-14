#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjects/GameObject.h"
#include <set>
#include <QUrl>

namespace Weapons { enum class ProjectileProperty; }

namespace GameObjects {
namespace Projectiles {

class Projectile : public GameObject {
public:
  Projectile();
    Projectile(bool hostile = false, int damage = 1, std::set<Weapons::ProjectileProperty> properties = {});
  virtual ~Projectile() = default;
  virtual std::unique_ptr<Projectile> clone() const = 0;

  bool shouldBeDeleted() override;
  void collideWith(GameObject &other) override;
  void collideWithEnemyShip(Ships::EnemyShip &enemyShip) override;
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override;
  int getDamage() const;

  bool hostile() const;
  void setHostile(bool newHostile);

  void setDamage(int newDamage);

  void setProperties(const std::set<Weapons::ProjectileProperty> &newProperties);
  void addProperty(const Weapons::ProjectileProperty property);
  void removeProperty(const Weapons::ProjectileProperty property);

protected:
  bool m_hostile;
  int m_damage;
  std::set<Weapons::ProjectileProperty> m_properties;
};
} // namespace Projectiles
} // namespace GameObjects

#endif // PROJECTILE_H
