#ifndef GAME_LEVELS_SPAWNEVENT_H
#define GAME_LEVELS_SPAWNEVENT_H

#include "Game/Levels/Formation.h"
#include "GameObjects/GameObjectBlueprint.h"
#include "GameObjects/GameObject.h"
#include <functional>
#include <memory>

namespace Game {
namespace Levels {

class SpawnEvent {
public:
  SpawnEvent(Config::GameContext ctx);
  void execute(int elapsedTimeMs,
               std::function<void(std::shared_ptr<GameObjects::GameObject>)>
                   addGameObjectFunc);
  bool isFinished() const;

  SpawnEvent &withTriggerTime(int triggerTimeMs);
  SpawnEvent &withPosition(QVector2D position);
  SpawnEvent &withPositionRange(QVector2D minPosition, QVector2D maxPosition);
  SpawnEvent &withCount(int count);
  SpawnEvent &withInterval(int intervalMs);
  SpawnEvent &withFormation(const Formation formation);
  SpawnEvent &
  withGameObjectBlueprint(GameObjects::GameObjectBlueprint & gameObjectBlueprint);

private:
  Config::GameContext m_gameCtx;
  GameObjects::GameObjectBlueprint m_gameObjectBlueprint;
  Formation m_formation;
  std::pair<QVector2D, QVector2D> m_positionRange;
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
