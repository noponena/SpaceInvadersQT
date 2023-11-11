#ifndef GAMEOBJECTS_PROJECTILES_BASICENEMYLASER_H
#define GAMEOBJECTS_PROJECTILES_BASICENEMYLASER_H

#include "LaserBeam.h"

namespace GameObjects {
namespace Projectiles {

class BasicEnemyLaser : public GameObjects::Projectiles::LaserBeam
{
public:
    BasicEnemyLaser(Position position, float speed, bool hostile = false, int damage = 1, std::set<Weapons::WeaponProperty> properties = {});
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_BASICENEMYLASER_H
