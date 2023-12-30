#ifndef GAME_LEVELS_LEVELLOADER_H
#define GAME_LEVELS_LEVELLOADER_H

#include "Formation.h"
#include "Game/Core/GameState.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "GameObjects/Ships/ShipBuilder.h"
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
  GameObjects::Ships::ShipBuilder m_shipBuilder;
  int m_screenWidth;
  int m_screenHeight;
  int m_minX;
  int m_maxX;
  int m_minY;
  int m_maxY;

  Formation::Type stringToFormationType(std::string formationTypeStr);
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_LEVELLOADER_H
