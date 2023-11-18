#include "LevelManager.h"
#include "GameObjects/Projectiles/EnemyLaserProjectile.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Utils/Utils.h"
#include "Weapons/PrimaryWeapon.h"

namespace Game {
namespace Core {
LevelManager::LevelManager(GameState *gameState, bool performanceTest)
    : m_gameState(gameState), m_lastSpawnTime(0) {
  m_elapsedTimer.start();
  if (performanceTest) {
    m_spawnIntervalMs = 50;
    m_enemyWeaponCooldownMs = 500;
    m_enemyShipHp = 3;
  }
}

void LevelManager::update() {
  float currentTime = m_elapsedTimer.elapsed();
  if (currentTime - m_lastSpawnTime >= m_spawnIntervalMs) {

    int randomX = Utils::randi(m_gameState->m_minX, m_gameState->m_maxX);
    int y = m_gameState->m_minY - 50;
    int minX = m_gameState->m_minX;
    int maxX = m_gameState->m_maxX;
    int minY = m_gameState->m_minY;
    int maxY = m_gameState->m_maxY;

    // 2. Create a new enemy ship
    GameObjects::Position pos(randomX, y, minX, maxX, minY, maxY);
    std::unique_ptr<GameObjects::Ships::EnemyShip> enemyShip =
        std::make_unique<GameObjects::Ships::EnemyShip>(m_enemyShipHp, pos);
    enemyShip->initialize();

    std::unique_ptr<Weapons::Weapon> weapon =
        m_weaponBuilder.createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
            .withProjectileDamage(1)
            .withProjectile(std::make_unique<
                            GameObjects::Projectiles::EnemyLaserProjectile>())
            .withProjectileMovementStrategy(
                Game::Movement::VerticalMovementStrategy(500, 1))
            .withWeaponCooldownMs(m_enemyWeaponCooldownMs)
            .build();

    enemyShip->addWeapon(std::move(weapon));
    enemyShip->setAutoShoot(true);

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
    m_gameState->addGameObject(std::move(enemyShip));

    // 4. Restart the timer and update the last spawn time
    m_elapsedTimer.restart();
    m_lastSpawnTime = 0;
  }
}
} // namespace Core
} // namespace Game
