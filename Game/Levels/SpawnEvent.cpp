#include "SpawnEvent.h"
#include "Utils/Utils.h"

namespace Game {
namespace Levels {

SpawnEvent::SpawnEvent()
    : m_count(1), m_triggered(false), m_finished(false), m_lastSpawnTimeMs(0),
      m_spawnCounter(0) {}

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
    qDebug() << "x=" << x;
    qDebug() << "y=" << y;
    std::vector<QVector2D> coordinates = m_formation.getPoints(QVector2D(x, y));
    for (QVector2D &point : coordinates) {
      std::unique_ptr<GameObjects::GameObject> clonedObject =
          m_gameObject->clone();
      clonedObject->moveAbsolute(QVector2D(point));
      qDebug() << "spawned enemy at " << point;
      addGameObjectFunc(std::move(clonedObject));
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

SpawnEvent &SpawnEvent::withGameObject(
    std::shared_ptr<GameObjects::GameObject> gameObject) {
  m_gameObject = std::move(gameObject);
  return *this;
}

} // namespace Levels
} // namespace Game
