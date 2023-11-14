#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <thread>
#include <variant>

#include <QDebug>

namespace Game {
namespace Movement {
class LinearMovement;
class SinusoidMovement;
class StationaryMovement;
class IntervalMovement;
} // namespace Movement
} // namespace Game

using AxisMovementStrategy = std::variant<
    Game::Movement::LinearMovement, Game::Movement::SinusoidMovement,
    Game::Movement::StationaryMovement, Game::Movement::IntervalMovement>;

namespace Game {

namespace Movement {

class LinearMovement {
  float m_speed;
  int m_direction;
  bool m_updateAnchor;

public:
  LinearMovement(float speed, int direction = 1, bool updateAnchor = false)
      : m_speed(speed), m_direction(direction), m_updateAnchor(updateAnchor) {}
  std::pair<float, float> move(float currentPosition, float anchorPosition,
                               float deltaTimeInSeconds) {
    float delta = m_speed * deltaTimeInSeconds * m_direction;
    if (m_updateAnchor)
      anchorPosition += delta;
    return {currentPosition + delta, anchorPosition};
  }
};

class SinusoidMovement {
  float m_amplitude, m_frequency, m_accumulatedTime, m_phase;
  int m_direction;
  bool m_updateAnchor;

public:
  SinusoidMovement(float amplitude, float frequency, float phase,
                   int direction = 1, bool updateAnchor = false)
      : m_amplitude(amplitude), m_frequency(frequency), m_accumulatedTime(0),
        m_phase(phase), m_direction(direction), m_updateAnchor(updateAnchor) {}

  std::pair<float, float> move(float currentPosition, float anchorPosition,
                               float deltaTimeInSeconds) {
    Q_UNUSED(currentPosition);
    m_accumulatedTime += deltaTimeInSeconds;
    float delta =
        m_amplitude *
        std::sin(2 * M_PI * m_frequency * m_accumulatedTime + m_phase * M_PI) *
        m_direction;
    if (m_updateAnchor)
      anchorPosition += delta;
    return {anchorPosition + delta, anchorPosition};
  }
};

class StationaryMovement {
public:
  std::pair<float, float> move(float currentPosition, float anchorPosition,
                               float deltaTimeInSeconds) {
    Q_UNUSED(deltaTimeInSeconds);
    return {currentPosition, anchorPosition};
  }
};

class IntervalMovement {
  std::vector<AxisMovementStrategy> m_underlyingStrategies;
  std::vector<float> m_intervalsInSeconds;
  float m_timeSinceLastChange;
  unsigned long long m_strategyIndex;
  unsigned long long m_intervalIndex;

public:
  IntervalMovement(std::vector<AxisMovementStrategy> strategies,
                   std::vector<float> intervalsInSeconds)
      : m_underlyingStrategies(strategies),
        m_intervalsInSeconds(intervalsInSeconds), m_timeSinceLastChange(0),
        m_strategyIndex(0), m_intervalIndex(0) {}

  IntervalMovement(std::vector<AxisMovementStrategy> strategies,
                   float intervalInSeconds)
      : m_underlyingStrategies(strategies),
        m_intervalsInSeconds(std::vector({intervalInSeconds})),
        m_timeSinceLastChange(0), m_strategyIndex(0), m_intervalIndex(0) {}

  std::pair<float, float> move(float currentPosition, float anchorPosition,
                               float deltaTimeInSeconds) {
    m_timeSinceLastChange += deltaTimeInSeconds;
    AxisMovementStrategy underlyingStrategy =
        m_underlyingStrategies[m_strategyIndex];
    float intervalInSeconds = m_intervalsInSeconds[m_intervalIndex];

    if (m_timeSinceLastChange > intervalInSeconds) {
      m_timeSinceLastChange = 0;
      incrementStrategyIndex();
      incrementIntervalIndex();
    }

    return std::visit(
        [currentPosition, anchorPosition, deltaTimeInSeconds](auto &strategy) {
          return strategy.move(currentPosition, anchorPosition,
                               deltaTimeInSeconds);
        },
        underlyingStrategy);
  }

private:
  void incrementStrategyIndex() {
    m_strategyIndex++;
    if (m_strategyIndex >= m_underlyingStrategies.size())
      m_strategyIndex = 0;
  }
  void incrementIntervalIndex() {
    m_intervalIndex++;
    if (m_intervalIndex >= m_intervalsInSeconds.size())
      m_intervalIndex = 0;
  }
};

} // namespace Movement

} // namespace Game
