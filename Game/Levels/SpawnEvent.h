#ifndef GAME_LEVELS_SPAWNEVENT_H
#define GAME_LEVELS_SPAWNEVENT_H

#include "Game/Levels/Formation.h"
#include "GameObjects/GameObject.h"
#include <functional>
#include <memory>

namespace Game {
namespace Levels {

class SpawnEvent {
public:
  SpawnEvent(std::function<void(std::shared_ptr<GameObjects::GameObject>)>
                 addGameObjectFunction);
  void execute(int elapsedTimeMs);
  bool isFinished() const;

  SpawnEvent &withTriggerTime(int triggerTimeMs);
  SpawnEvent &withPosition(QPoint position);
  SpawnEvent &withCount(int count);
  SpawnEvent &withInterval(int intervalMs);
  SpawnEvent &withFormation(const Formation formation);
  SpawnEvent &
  withGameObject(std::shared_ptr<GameObjects::GameObject> gameObject);

private:
  std::function<void(std::shared_ptr<GameObjects::GameObject>)>
      m_addGameObjectFunc;
  std::shared_ptr<GameObjects::GameObject> m_gameObject;
  Formation m_formation;
  QPoint m_position;
  int m_triggerTimeMs;
  int m_count;
  int m_intervalMs;
  bool m_triggered;
  bool m_finished;
  int m_lastSpawnTimeMs;
  int m_spawnCounter;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_SPAWNEVENT_H
