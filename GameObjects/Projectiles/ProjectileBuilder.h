#ifndef GAMEOBJECTS_PROJECTILES_PROJECTILEBUILDER_H
#define GAMEOBJECTS_PROJECTILES_PROJECTILEBUILDER_H

#include "GameObjects/Projectiles/Projectile.h"

namespace GameObjects {
namespace Projectiles {

class ProjectileBuilder {
public:
  ProjectileBuilder();
  ProjectileBuilder clone() const;

  template <typename ProjectileType> ProjectileBuilder &createProjectile(Config::GameContext ctx) {
    m_projectile = std::make_unique<ProjectileType>(ctx);
    m_projectile->setMovementStrategy(
        Game::Movement::StationaryMovementStrategy());
    m_projectile->setDamage(1);
    return *this;
  }

  ProjectileBuilder &withObjectType(const ObjectType objectType);
  ProjectileBuilder &withDamage(const int damage);
  ProjectileBuilder &
  withMovementStrategy(const Game::Movement::MovementStrategy &strategy);
  ProjectileBuilder &withProperty(const ProjectileProperty property);
  ProjectileBuilder &withSpawnSound(const Game::Audio::SoundInfo spawnSound);
  ProjectileBuilder &
  withDestructionSound(const Game::Audio::SoundInfo destructionSound);
  ProjectileBuilder &withGrahpics(const RenderDataMap renderDataMap);
  std::unique_ptr<Projectile> build();

private:
  std::unique_ptr<Projectile> m_projectile;
  void logNullPointerWarning() const;
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_PROJECTILEBUILDER_H
