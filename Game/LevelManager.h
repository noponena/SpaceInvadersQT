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
  QElapsedTimer m_elapsedTimer;
  GameState &m_gameState;
  float m_lastSpawnTime = 0.0f;
  const int m_spawnIntervalMs = 2500;
};
} // namespace Game

#endif // LEVELMANAGER_H
