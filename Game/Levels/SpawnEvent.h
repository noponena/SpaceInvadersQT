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
  SpawnEvent();
  void execute(int elapsedTimeMs,
               std::function<void(std::shared_ptr<GameObjects::GameObject>)>
                   addGameObjectFunc);
  bool isFinished() const;

  SpawnEvent &withTriggerTime(int triggerTimeMs);
  SpawnEvent &withPosition(QPoint position);
  SpawnEvent &withPositionRange(QPoint minPosition, QPoint maxPosition);
  SpawnEvent &withCount(int count);
  SpawnEvent &withInterval(int intervalMs);
  SpawnEvent &withFormation(const Formation formation);
  SpawnEvent &
  withGameObject(std::shared_ptr<GameObjects::GameObject> gameObject);

private:
  std::shared_ptr<GameObjects::GameObject> m_gameObject;
  Formation m_formation;
  std::pair<QPoint, QPoint> m_positionRange;
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
