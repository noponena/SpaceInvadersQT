#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include "AxisMovementStrategy.h"
#include "Utils/Math/MathConstants.h"
#include <QVector2D>
#include <tuple>
#include <variant>
#include <vector>

namespace Game {
namespace Movement {

class MovementStrategy {

public:
  MovementStrategy() {
    m_xAxisMovementStrategies.reserve(m_maxStrategies);
    m_yAxisMovementStrategies.reserve(m_maxStrategies);
  }

  std::pair<QVector2D, QVector2D> move(QVector2D pos, QVector2D anchorPos,
                                       float deltaTimeInSeconds) {
    float x = pos.x();
    float y = pos.y();
    float anchorX = anchorPos.x();
    float anchorY = anchorPos.y();
    for (auto &strategy : m_xAxisMovementStrategies) {
      std::tie(x, anchorX) = std::visit(
          [&](auto &&arg) { return arg.move(x, anchorX, deltaTimeInSeconds); },
          strategy);
    }
    for (auto &strategy : m_yAxisMovementStrategies) {
      std::tie(y, anchorY) = std::visit(
          [&](auto &&arg) { return arg.move(y, anchorY, deltaTimeInSeconds); },
          strategy);
    }
    return {QVector2D(x, y), QVector2D(anchorX, anchorY)};
  }

  void clear() {
    m_xAxisMovementStrategies.clear();
    m_yAxisMovementStrategies.clear();
  }

  std::vector<AxisMovementStrategy> xAxisMovementStrategies() const {
    return m_xAxisMovementStrategies;
  }

  std::vector<AxisMovementStrategy> yAxisMovementStrategies() const {
    return m_yAxisMovementStrategies;
  }

  MovementStrategy operator+(const MovementStrategy &other) const {
    std::vector<AxisMovementStrategy> combinedXStrategies;
    combinedXStrategies.reserve(m_xAxisMovementStrategies.size() +
                                other.m_xAxisMovementStrategies.size());
    combinedXStrategies.insert(combinedXStrategies.end(),
                               m_xAxisMovementStrategies.begin(),
                               m_xAxisMovementStrategies.end());
    combinedXStrategies.insert(combinedXStrategies.end(),
                               other.m_xAxisMovementStrategies.begin(),
                               other.m_xAxisMovementStrategies.end());

    std::vector<AxisMovementStrategy> combinedYStrategies;
    combinedYStrategies.reserve(m_yAxisMovementStrategies.size() +
                                other.m_yAxisMovementStrategies.size());
    combinedYStrategies.insert(combinedYStrategies.end(),
                               m_yAxisMovementStrategies.begin(),
                               m_yAxisMovementStrategies.end());
    combinedYStrategies.insert(combinedYStrategies.end(),
                               other.m_yAxisMovementStrategies.begin(),
                               other.m_yAxisMovementStrategies.end());

    MovementStrategy combined;
    combined.m_xAxisMovementStrategies = std::move(combinedXStrategies);
    combined.m_yAxisMovementStrategies = std::move(combinedYStrategies);

    return combined;
  }

protected:
  std::vector<AxisMovementStrategy> m_xAxisMovementStrategies;
  std::vector<AxisMovementStrategy> m_yAxisMovementStrategies;

private:
  int m_maxStrategies = 10;
};

class StationaryMovementStrategy : public MovementStrategy {
public:
  StationaryMovementStrategy() {
    m_xAxisMovementStrategies.push_back(Movement::StationaryMovement());
    m_yAxisMovementStrategies.push_back(Movement::StationaryMovement());
  }
};

class VerticalMovementStrategy : public MovementStrategy {

public:
  VerticalMovementStrategy(float speed = 1, int direction = 1) {
    m_yAxisMovementStrategies.push_back(
        Movement::LinearMovement(speed, direction));
  }
};

class HorizontalMovementStrategy : public MovementStrategy {

public:
  HorizontalMovementStrategy(float speed = 1, int direction = 1) {
    m_xAxisMovementStrategies.push_back(
        Movement::LinearMovement(speed, direction));
  }
};

class AngledMovementStrategy : public MovementStrategy {

public:
  AngledMovementStrategy(float speed = 1, int direction = 1, int angleDeg = 0) {

    float angleRad = angleDeg * PI / 180.0;
    float speedX = sin(angleRad) * speed;
    float speedY = -cos(angleRad) * speed;

    m_xAxisMovementStrategies.push_back(
        Movement::LinearMovement(speedX, direction));
    m_yAxisMovementStrategies.push_back(
        Movement::LinearMovement(speedY, direction));
  }
};

class CircularMovementStrategy : public MovementStrategy {

public:
  CircularMovementStrategy(float amplitude, float frequency, int direction = 1,
                           bool updateAnchor = false) {
    m_xAxisMovementStrategies.push_back(Movement::SinusoidMovement(
        amplitude, frequency, 0, direction, updateAnchor));
    m_yAxisMovementStrategies.push_back(Movement::SinusoidMovement(
        amplitude, frequency, 0.5, direction, false));
  }
};

class IntervalMovementStrategy : public MovementStrategy {

public:
  IntervalMovementStrategy(MovementStrategy strategy, float intervalInSeconds) {
    std::vector<AxisMovementStrategy> xStrategies =
        strategy.xAxisMovementStrategies();
    std::vector<AxisMovementStrategy> yStrategies =
        strategy.yAxisMovementStrategies();

    if (!xStrategies.empty())
      m_xAxisMovementStrategies.push_back(
          Movement::IntervalMovement(xStrategies, intervalInSeconds));
    if (!yStrategies.empty())
      m_yAxisMovementStrategies.push_back(
          Movement::IntervalMovement(yStrategies, intervalInSeconds));
  }

  IntervalMovementStrategy(
      std::vector<std::pair<MovementStrategy, float>> strategies) {

    size_t xReserve = 0, yReserve = 0;
    for (const auto &pair : strategies) {
      xReserve += pair.first.xAxisMovementStrategies().size();
      yReserve += pair.first.yAxisMovementStrategies().size();
    }

    std::vector<AxisMovementStrategy> xMovementStrategies;
    std::vector<AxisMovementStrategy> yMovementStrategies;
    std::vector<float> xIntervals(xReserve, 0);
    std::vector<float> yIntervals(yReserve, 0);

    size_t xIndex = 0, yIndex = 0;
    for (const auto &pair : strategies) {
      std::vector<AxisMovementStrategy> xStrategies =
          pair.first.xAxisMovementStrategies();
      std::vector<AxisMovementStrategy> yStrategies =
          pair.first.yAxisMovementStrategies();

      xMovementStrategies.insert(xMovementStrategies.end(), xStrategies.begin(),
                                 xStrategies.end());
      yMovementStrategies.insert(yMovementStrategies.end(), yStrategies.begin(),
                                 yStrategies.end());

      std::fill_n(xIntervals.begin() + xIndex, xStrategies.size(), pair.second);
      std::fill_n(yIntervals.begin() + yIndex, yStrategies.size(), pair.second);
      xIndex += xStrategies.size();
      yIndex += yStrategies.size();
    }

    if (!xMovementStrategies.empty())
      m_xAxisMovementStrategies.push_back(
          Movement::IntervalMovement(xMovementStrategies, xIntervals));
    if (!yMovementStrategies.empty())
      m_yAxisMovementStrategies.push_back(
          Movement::IntervalMovement(yMovementStrategies, yIntervals));
  }
};

class DownwardMovingCircularMovementStrategy : public MovementStrategy {

public:
  DownwardMovingCircularMovementStrategy(float amplitude, float frequency,
                                         float speed, int direction = 1) {
    m_xAxisMovementStrategies.push_back(
        Movement::SinusoidMovement(amplitude, frequency, 0, direction, false));
    m_yAxisMovementStrategies.push_back(Movement::SinusoidMovement(
        amplitude, frequency, 0.5, direction, false));
    m_yAxisMovementStrategies.push_back(
        Movement::LinearMovement(speed, 1, true));
  }
};

} // namespace Movement
} // namespace Game

#endif // MOVEMENTSTRATEGY_H
