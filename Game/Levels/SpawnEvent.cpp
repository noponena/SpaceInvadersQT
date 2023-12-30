#include "SpawnEvent.h"

namespace Game {
namespace Levels {

SpawnEvent::SpawnEvent(
    std::function<void(std::shared_ptr<GameObjects::GameObject>)>
        addGameObjectFunction)
    : m_addGameObjectFunc(addGameObjectFunction), m_count(1),
      m_triggered(false), m_finished(false), m_lastSpawnTimeMs(0),
      m_spawnCounter(0) {}

void SpawnEvent::execute(int elapsedTimeMs) {
  if (m_finished)
    return;

  if (!m_triggered)
    m_triggered = elapsedTimeMs >= m_triggerTimeMs;

  if (m_triggered && m_count > m_spawnCounter &&
      (elapsedTimeMs - m_lastSpawnTimeMs) >= m_intervalMs) {
    std::vector<QPoint> coordinates = m_formation.getPoints(m_position);
    for (QPoint &point : coordinates) {
      std::unique_ptr<GameObjects::GameObject> clonedObject =
          m_gameObject->clone();
      clonedObject->setPosition(point);
      m_addGameObjectFunc(std::move(clonedObject));
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

SpawnEvent &SpawnEvent::withPosition(QPoint position) {
  m_position = position;
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

SpawnEvent &SpawnEvent::withGameObject(
    std::shared_ptr<GameObjects::GameObject> gameObject) {
  m_gameObject = std::move(gameObject);
  return *this;
}

} // namespace Levels
} // namespace Game
