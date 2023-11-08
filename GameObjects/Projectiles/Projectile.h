#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjects/GameObject.h"
#include <QUrl>

namespace GameObjects {
namespace Projectiles {

class Projectile : public GameObject {
public:
  Projectile(Position pos, float speed, bool hostile = false, int damage = 1);
  virtual ~Projectile() = default;

  bool shouldBeDeleted() override;
  void collideWith(GameObject &other) override;
  void collideWithEnemyShip(Ships::EnemyShip &enemyShip) override;
  int getDamage() const;

  bool hostile() const;

  protected:
  bool m_hostile;

private:
  int m_damage;
};
} // namespace Projectiles
} // namespace GameObjects

#endif // PROJECTILE_H
