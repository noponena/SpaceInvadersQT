#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Game/Core/GameState.h"
#include "Game/Levels/Level.h"
#include "Game/Levels/LevelLoader.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"

namespace Game {
namespace Core {
class LevelManager : public QObject {
  Q_OBJECT
public:
  LevelManager(GameState *gameState, bool performanceTest = false);
  LevelManager(GameState *gameState, int screenWidth, int screenHeight);
  void startLevel(int levelNumber);
  void update();
  void progressLevel();
  void setLevel(int levelNumber);
  void start();

private:
  std::unique_ptr<Levels::LevelLoader> m_levelLoader;
  std::unordered_map<int, Game::Levels::Level> m_levels;
  Game::Levels::Level m_currentLevel;
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder m_projectileBuilder;
  QElapsedTimer m_elapsedTimer;
  QElapsedTimer m_levelTimer;
  GameState *m_gameState;
  float m_lastSpawnTime = 0.0f;
  int m_spawnIntervalMs = 1000;
  int m_enemyWeaponCooldownMs = 2500;
  int m_enemyShipHp = 5;
  int m_enemyShipDestructionParticleCount = 250;
  bool m_performanceTest;
};
} // namespace Core
} // namespace Game

#endif // LEVELMANAGER_H
