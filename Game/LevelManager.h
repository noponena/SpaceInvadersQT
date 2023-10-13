#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "GameState.h"
#include "GameObjects/Ships/EnemyShip.h"

namespace Game {
class LevelManager
{
public:
    LevelManager(GameState& gameState);
    void update();
private:
    QElapsedTimer m_elapsedTimer;
    GameState& m_gameState;
    float m_lastSpawnTime = 0.0f;
    const int m_spawnFrequency = 500;
};
}



#endif // LEVELMANAGER_H
