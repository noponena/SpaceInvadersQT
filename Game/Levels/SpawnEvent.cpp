#include "SpawnEvent.h"
#include "Utils/Utils.h"

namespace Game {
namespace Levels {

SpawnEvent::SpawnEvent(Config::GameContext ctx)
    : m_gameCtx(ctx)
    , m_count(1)
    , m_triggered(false)
    , m_finished(false)
    , m_lastSpawnTimeMs(0)
    , m_spawnCounter(0)
{}

void SpawnEvent::execute(
    int elapsedTimeMs,
    std::function<void(std::shared_ptr<GameObjects::GameObject>)>
        addGameObjectFunc) {
  if (m_finished)
    return;

  if (!m_triggered)
    m_triggered = elapsedTimeMs >= m_triggerTimeMs;

  if (m_triggered && m_count > m_spawnCounter &&
      (elapsedTimeMs - m_lastSpawnTimeMs) >= m_intervalMs) {

    int x = Utils::randi(m_positionRange.first.x(), m_positionRange.second.x());
    int y = Utils::randi(m_positionRange.first.y(), m_positionRange.second.y());
    std::vector<QVector2D> coordinates = m_formation.getPoints(QVector2D(x, y));
    for (QVector2D &point : coordinates) {
      std::unique_ptr<GameObjects::GameObject> concreteObject =
            m_gameObjectBlueprint.instantiate(m_gameCtx);
      concreteObject->initialize();
      concreteObject->moveAbsolute(QVector2D(point));
      addGameObjectFunc(std::move(concreteObject));
    }
    m_lastSpawnTimeMs = elapsedTimeMs;
    m_spawnCounter++;
  }

  m_finished = m_spawnCounter == m_count;
}

bool SpawnEvent::isFinished() const { return m_finished; }

SpawnEvent &SpawnEvent::withTriggerTime(int triggerTimeMs) {
  m_triggerTimeMs = triggerTimeMs;
  return *this;
}

SpawnEvent &SpawnEvent::withPosition(QVector2D position) {
  m_positionRange = {position, position};
  return *this;
}

SpawnEvent &SpawnEvent::withPositionRange(QVector2D minPosition,
                                          QVector2D maxPosition) {
  m_positionRange = {minPosition, maxPosition};
  return *this;
}

SpawnEvent &SpawnEvent::withCount(int count) {
  m_count = count;
  return *this;
}

SpawnEvent &SpawnEvent::withInterval(int intervalMs) {
  m_intervalMs = intervalMs;
  return *this;
}

SpawnEvent &SpawnEvent::withFormation(const Formation formation) {
  m_formation = formation;
  return *this;
}

SpawnEvent &SpawnEvent::withGameObjectBlueprint(GameObjects::GameObjectBlueprint & gameObjectBlueprint)
{
    m_gameObjectBlueprint = gameObjectBlueprint;
    return *this;
}

} // namespace Levels
} // namespace Game
