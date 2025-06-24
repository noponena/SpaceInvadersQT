#ifndef GAMEOBJECTS_PROJECTILES_VORTEX_H
#define GAMEOBJECTS_PROJECTILES_VORTEX_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {

class Vortex : public Projectile {
public:
  Vortex();

private:
  float m_timeToLiveSeconds;
  float m_timeSinceSpawnSeconds;

public:
  void update(const UpdateContext &context) override;
  void collideWithEnemyShip(Ships::EnemyShip &enemyShip) override;
  bool shouldBeDeleted() const override;
  std::unique_ptr<GameObject> clone() const override;
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_VORTEX_H
