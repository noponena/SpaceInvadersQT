#ifndef GAMEOBJECTS_PROJECTILES_PROJECTILEBUILDER_H
#define GAMEOBJECTS_PROJECTILES_PROJECTILEBUILDER_H

#include "GameObjects/Projectiles/Projectile.h"

namespace GameObjects {
namespace Projectiles {

class ProjectileBuilder {
public:
  ProjectileBuilder();
  ProjectileBuilder clone() const;
  ProjectileBuilder &createProjectile(std::unique_ptr<Projectile> projectile);
  ProjectileBuilder &withObjectType(const ObjectType objectType);
  ProjectileBuilder &withDamage(const int damage);
  ProjectileBuilder &
  withMovementStrategy(const Game::Movement::MovementStrategy &strategy);
  ProjectileBuilder &withProperty(const ProjectileProperty property);
  ProjectileBuilder &withSpawnSound(const Game::Audio::SoundInfo spawnSound);
  ProjectileBuilder &
  withDestructionSound(const Game::Audio::SoundInfo destructionSound);
  ProjectileBuilder &withGrahpics(const GameObjects::PixmapData pixmapData);
  std::unique_ptr<GameObjects::Projectiles::Projectile> build();

private:
  std::unique_ptr<Projectile> m_projectile;
  void logNullPointerWarning() const;
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_PROJECTILEBUILDER_H
