#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "Game/Core/GameState.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"

namespace Game {
namespace Core {
class LevelManager : public QObject {
  Q_OBJECT
public:
  LevelManager(GameState *gameState, bool performanceTest = false);
  void update();

private:
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder m_projectileBuilder;
  QElapsedTimer m_elapsedTimer;
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
