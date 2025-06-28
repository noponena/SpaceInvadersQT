#ifndef GAME_LEVELS_LEVELLOADER_H
#define GAME_LEVELS_LEVELLOADER_H

#include "Formation.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "GameObjects/Ships/ShipBuilder.h"
#include "Level.h"
#include "Weapons/WeaponBuilder.h"

namespace Game {
namespace Levels {

class LevelLoader {
public:
  LevelLoader() = default;
  void initialize();
  Level loadLevel(const std::string &filepath) const;
  std::map<int, Level> loadLevels() const;
  Level loadBenchmarkLevel();
  void setGameCtx(Config::GameContext &ctx);

private:
  std::unique_ptr<Config::GameContext> m_gameCtx;
  std::unique_ptr<GameObjects::Ships::EnemyShip> m_enemyShip;
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder m_projectileBuilder;
  GameObjects::Ships::ShipBuilder m_shipBuilder;

  Formation::Type stringToFormationType(std::string formationTypeStr) const;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_LEVELLOADER_H
