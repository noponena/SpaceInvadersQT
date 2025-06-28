#ifndef GAMEOBJECTS_PROJECTILES_PLAYERPROJECTILE_H
#define GAMEOBJECTS_PROJECTILES_PLAYERPROJECTILE_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {

class PlayerProjectile : public Projectile {
public:
  PlayerProjectile(Config::GameContext ctx);
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_PLAYERPROJECTILE_H
