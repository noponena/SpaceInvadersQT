#ifndef GAME_LEVELS_LEVELLOADER_H
#define GAME_LEVELS_LEVELLOADER_H

#include "Game/Core/GameState.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Level.h"
#include "Weapons/WeaponBuilder.h"
#include <unordered_map>

namespace Game {
namespace Levels {

class LevelLoader {
public:
  LevelLoader(Core::GameState *gameState, int screenWidth, int screenHeight);
  Level loadLevel(const std::string &filepath);
  std::unordered_map<int, Level> loadLevels();

private:
  std::unique_ptr<GameObjects::Ships::EnemyShip> m_enemyShip;
  Core::GameState *m_gameState;
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder m_projectileBuilder;
  int m_screenWidth;
  int m_screenHeight;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_LEVELLOADER_H
