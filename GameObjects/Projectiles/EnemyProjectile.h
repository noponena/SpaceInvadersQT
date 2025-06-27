#ifndef GAMEOBJECTS_PROJECTILES_ENEMYPROJECTILE_H
#define GAMEOBJECTS_PROJECTILES_ENEMYPROJECTILE_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {

class EnemyProjectile : public Projectile {
public:
  EnemyProjectile(Config::GameContext ctx);
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_ENEMYPROJECTILE_H
