#ifndef GAMEOBJECTS_PROJECTILES_BASICENEMYLASER_H
#define GAMEOBJECTS_PROJECTILES_BASICENEMYLASER_H

#include "LaserBeam.h"

namespace GameObjects {
namespace Projectiles {

class LesserEnemyLaserBeam : public GameObjects::Projectiles::LaserBeam
{
public:
    LesserEnemyLaserBeam(bool hostile = false, int damage = 1, std::set<Weapons::ProjectileProperty> properties = {});
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_BASICENEMYLASER_H
