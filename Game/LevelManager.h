#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "GameObjects/Ships/EnemyShip.h"
#include "GameState.h"

namespace Game {
class LevelManager : public QObject {
    Q_OBJECT
public:
  LevelManager(GameState &gameState);
  void update();

private:
  QElapsedTimer m_elapsedTimer;
  GameState &m_gameState;
  float m_lastSpawnTime = 0.0f;
  const int m_spawnFrequency = 500;
};
} // namespace Game

#endif // LEVELMANAGER_H
