#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "GameState.h"

namespace Game {
class LevelManager : public QObject {
    Q_OBJECT
public:
  LevelManager(GameState &gameState);
  void update();

private:
  Weapons::WeaponBuilder m_weaponBuilder;
  QElapsedTimer m_elapsedTimer;
  GameState &m_gameState;
  float m_lastSpawnTime = 0.0f;
  const int m_spawnIntervalMs = 1000;
};
} // namespace Game

#endif // LEVELMANAGER_H
