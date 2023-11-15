#include "LevelManager.h"
#include "GameObjects/Projectiles/EnemyLaserBeam.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/PrimaryWeapon.h"
#include <random>

namespace Game {

LevelManager::LevelManager(GameState &gameState)
    : m_gameState(gameState), m_lastSpawnTime(0) {
  m_elapsedTimer.start();
}

void LevelManager::update() {
  float currentTime = m_elapsedTimer.elapsed();
  if (currentTime - m_lastSpawnTime >= m_spawnIntervalMs) {
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
    std::unique_ptr<GameObjects::Ships::EnemyShip> enemyShip =
        std::make_unique<GameObjects::Ships::EnemyShip>(5, pos);
    enemyShip->initialize();

    std::unique_ptr<Weapons::Weapon> weapon =
        m_weaponBuilder.createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
            .withProjectileDamage(1)
            .withProjectile(
                std::make_unique<GameObjects::Projectiles::EnemyLaserBeam>())
            .withProjectileMovementStrategy(
                Game::Movement::VerticalMovementStrategy(500, 1))
            .withWeaponCooldownMs(2000)
            .build();

    enemyShip->addWeapon(std::move(weapon));

    Game::Movement::MovementStrategy horizontalStrategyLeft =
        Game::Movement::HorizontalMovementStrategy(200, -1);
    Game::Movement::MovementStrategy horizontalStrategyRight =
        Game::Movement::HorizontalMovementStrategy(200, 1);
    Game::Movement::MovementStrategy horizontalCombined =
        horizontalStrategyLeft + horizontalStrategyRight;

    Game::Movement::MovementStrategy verticalStrategy =
        Game::Movement::VerticalMovementStrategy(200, 1);
    Game::Movement::MovementStrategy stationaryStrategy =
        Game::Movement::StationaryMovementStrategy();
    std::vector<std::pair<Game::Movement::MovementStrategy, float>>
        verticalCombined = {std::make_pair(verticalStrategy, 0.25f),
                            std::make_pair(stationaryStrategy, 3.0f)};

    Game::Movement::IntervalMovementStrategy horizontalIntervalStrategy =
        Game::Movement::IntervalMovementStrategy(horizontalCombined, 1.0f);
    Game::Movement::IntervalMovementStrategy verticalIntervalStrategy =
        Game::Movement::IntervalMovementStrategy(verticalCombined);

    Game::Movement::MovementStrategy combined =
        horizontalIntervalStrategy + verticalIntervalStrategy;

    //    enemyShip->setMovementStrategy(
    //        Game::Movement::VerticalMovementStrategy(100, 1));

    enemyShip->setMovementStrategy(combined);

    // 3. Add the enemy ship to the game state
    m_gameState.addGameObject(std::move(enemyShip));

    // 4. Restart the timer and update the last spawn time
    m_elapsedTimer.restart();
    m_lastSpawnTime = 0;
  }
}
} // namespace Game
