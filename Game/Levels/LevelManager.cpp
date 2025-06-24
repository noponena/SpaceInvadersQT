#include "LevelManager.h"
#include "Weapons/PrimaryWeapon.h"
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Levels {

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

    if (m_currentLevel.enemyLimit >= 0 &&
        m_gameState->enemyShipsReachedBottomLimit() > m_currentLevel.enemyLimit)
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
