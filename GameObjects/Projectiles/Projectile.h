#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjects/GameObject.h"
#include <set>
#include <QUrl>

namespace Weapons { enum class WeaponProperty; }

namespace GameObjects {
namespace Projectiles {

class Projectile : public GameObject {
public:
    Projectile(Position pos, float speed, bool hostile = false, int damage = 1, std::set<Weapons::WeaponProperty> properties = {});
  virtual ~Projectile() = default;

  bool shouldBeDeleted() override;
  void collideWith(GameObject &other) override;
  void collideWithEnemyShip(Ships::EnemyShip &enemyShip) override;
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override;
  int getDamage() const;

  bool hostile() const;

  protected:
  bool m_hostile;

private:
  int m_damage;
    std::set<Weapons::WeaponProperty> m_properties;
};
} // namespace Projectiles
} // namespace GameObjects

#endif // PROJECTILE_H
