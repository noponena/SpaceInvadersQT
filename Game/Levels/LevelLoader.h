#ifndef GAME_LEVELS_LEVELLOADER_H
#define GAME_LEVELS_LEVELLOADER_H

#include "Formation.h"
#include "Game/Movement/MovementStrategyLoader.h"
#include "GameObjects/GameObjectLoader.h"
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
  Game::Movement::MovementStrategyLoader m_movementLoader;
  GameObjects::GameObjectLoader m_gameObjectLoader;
  Weapons::WeaponBuilder m_weaponBuilder;
  Formation::Type stringToFormationType(std::string formationTypeStr) const;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_LEVELLOADER_H
