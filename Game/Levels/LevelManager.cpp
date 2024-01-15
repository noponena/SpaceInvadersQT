#include "LevelManager.h"
#include "Weapons/PrimaryWeapon.h"
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Levels {
LevelManager::LevelManager(Core::GameState *gameState, bool performanceTest)
    : m_gameState(gameState), m_lastSpawnTime(0), m_levelInProgress(false) {
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
          .createProjectile<GameObjects::Projectiles::Projectile>()
          .withDamage(1)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(500, 1))
          .withGrahpics(pixmapData)
          .withSpawnSound(Audio::SoundInfo(
              {true, Game::Audio::SoundEffect::LESSER_ENEMY_LASER}))
          .withObjectType(GameObjects::ObjectType::ENEMY_PROJECTILE)
          .build();

  m_weaponBuilder.createWeapon<Weapons::PrimaryWeapon>()
      .withProjectile(std::move(projectile))
      .withWeaponCooldownMs(m_enemyWeaponCooldownMs);
}

LevelManager::LevelManager(Core::GameState *gameState)
    : m_addGameObjectFunc(
          [this](auto object) { m_gameState->addGameObject(object); }),
      m_gameState(gameState), m_levelInProgress(false) {}

void LevelManager::startLevel() {
  m_levelInProgress = true;
  m_elapsedTimer.start();
}

void LevelManager::stopLevel() { m_levelInProgress = false; }

void LevelManager::progressLevel() {
  if (m_levelInProgress) {
    auto &spawnEvents = m_currentLevel.spawnEvents;
    for (auto it = spawnEvents.begin(); it != spawnEvents.end();) {
      it->execute(m_elapsedTimer.elapsed(), m_addGameObjectFunc);

      if (it->isFinished()) {
        it = spawnEvents.erase(it);
        qDebug() << "deleted spawn event";
      } else {
        ++it;
      }
    }

    if (m_gameState->enemyShipsReachedBottomLimit() > m_currentLevel.enemyLimit)
      emit enemyLimitReached();

    if (spawnEvents.empty())
      emit spawnEventsFinished();
  }
}

void LevelManager::setLevel(const Levels::Level &level) {
  m_currentLevel = level;
}

void LevelManager::start() { m_levelTimer.start(); }
} // namespace Levels
} // namespace Game
