#include "LevelManager.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Utils/Utils.h"
#include "Weapons/PrimaryWeapon.h"
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Core {
LevelManager::LevelManager(GameState *gameState, bool performanceTest)
    : m_gameState(gameState), m_lastSpawnTime(0) {
  m_elapsedTimer.start();
  if (performanceTest) {
    m_spawnIntervalMs = 50;
    m_enemyWeaponCooldownMs = 500;
    m_enemyShipHp = 3;
    m_enemyShipDestructionParticleCount = 250;
    m_performanceTest = performanceTest;
  }

  GameObjects::PixmapData pixmapData{
      QPointF(30, 30), ":/Images/enemy_laser_projectile.png", "", ""};

  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      m_projectileBuilder
          .createProjectile(
              std::make_unique<GameObjects::Projectiles::Projectile>())
          .withDamage(1)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(500, 1))
          .withGrahpics(pixmapData)
          .withSpawnSound(Audio::SoundInfo(
              {true, Game::Audio::SoundEffect::LESSER_ENEMY_LASER}))
          .withObjectType(GameObjects::ObjectType::ENEMY_PROJECTILE)
          .build();

  m_weaponBuilder.createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
      .withProjectile(std::move(projectile))
      .withWeaponCooldownMs(m_enemyWeaponCooldownMs);
}

LevelManager::LevelManager(GameState *gameState, int screenWidth,
                           int screenHeight)
    : m_gameState(gameState) {
  m_levelLoader = std::make_unique<Levels::LevelLoader>(
      m_gameState, screenWidth, screenHeight);
  m_levels = m_levelLoader->loadLevels();
}

void LevelManager::startLevel(int levelNumber) {
  m_currentLevel = m_levels[levelNumber];
  m_elapsedTimer.start();
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
    if (m_performanceTest)
      enemyShip->setDestructionParticleCount(
          m_enemyShipDestructionParticleCount);
    enemyShip->initialize();

    enemyShip->addPrimaryWeapon(m_weaponBuilder.build());
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

void LevelManager::progressLevel() {
  auto &spawnEvents = m_currentLevel.spawnEvents;
  for (auto it = spawnEvents.begin(); it != spawnEvents.end();) {
    it->execute(m_elapsedTimer.elapsed());

    if (it->isFinished()) {
      it = spawnEvents.erase(it);
      qDebug() << "deleted spawn event";
    } else {
      ++it;
    }
  }
}

void LevelManager::setLevel(int levelNumber) {
  m_currentLevel = m_levels.at(levelNumber);
}

void LevelManager::start() { m_levelTimer.start(); }
} // namespace Core
} // namespace Game
