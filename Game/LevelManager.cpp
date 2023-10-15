#include "LevelManager.h"
#include <random>

namespace Game {

LevelManager::LevelManager(GameState &gameState)
    : m_gameState(gameState), m_lastSpawnTime(0) {}

void LevelManager::update() {
  float currentTime = m_elapsedTimer.elapsed();
  if (currentTime - m_lastSpawnTime >= m_spawnFrequency) {
    // 1. Generate a random x position between minX and maxX
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(
        m_gameState.m_minX, m_gameState.m_maxX); // define the range

    int randomX = distr(eng);
    int y = m_gameState.m_minY - 50;
    int minX = m_gameState.m_minX;
    int maxX = m_gameState.m_maxX;
    int minY = m_gameState.m_minY;
    int maxY = m_gameState.m_maxY;

    // 2. Create a new enemy ship
    GameObjects::Position pos(randomX, y, minX, maxX, minY, maxY);
    std::shared_ptr<GameObjects::Ships::EnemyShip> enemyShip =
        std::make_shared<GameObjects::Ships::EnemyShip>(10, 100, pos);
    enemyShip->initialize();
    enemyShip->setMovementStrategy(
        Game::Movement::VerticalMovementStrategy(100, 1));

    // 3. Add the enemy ship to the game state
    m_gameState.addGameObject(enemyShip);

    // 4. Restart the timer and update the last spawn time
    m_elapsedTimer.restart();
    m_lastSpawnTime = 0;
  }
}
} // namespace Game
