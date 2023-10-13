#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cmath>
#include <variant>

#include <QDebug>

namespace Game {

namespace Movement {


class LinearMovement {
    int m_speed;
    int m_direction;
    bool m_updateAnchor;
public:
    LinearMovement(int speed, int direction = 1, bool updateAnchor = false) : m_speed(speed), m_direction(direction), m_updateAnchor(updateAnchor) {}
    std::pair<int, int> move(int currentPosition, int anchorPosition, float deltaTimeInSeconds) {
        int delta = static_cast<int>(m_speed * deltaTimeInSeconds) * m_direction;
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
    SinusoidMovement(float amplitude, float frequency, float phase, int direction = 1, bool updateAnchor = false)
        : m_amplitude(amplitude), m_frequency(frequency), m_accumulatedTime(0), m_phase(phase), m_direction(direction), m_updateAnchor(updateAnchor) {}

    std::pair<int, int> move(int currentPosition, int anchorPosition, float deltaTimeInSeconds) {
        Q_UNUSED(currentPosition);
        m_accumulatedTime += deltaTimeInSeconds;
        int delta = static_cast<int>(m_amplitude * std::sin(2 * M_PI * m_frequency * m_accumulatedTime + m_phase * M_PI)) * m_direction;
        if (m_updateAnchor)
            anchorPosition += delta;
        return {anchorPosition + delta, anchorPosition};
    }
};

class IntervalMovement {
    using UnderlyingStrategy = std::variant<LinearMovement, SinusoidMovement>;
    UnderlyingStrategy underlyingStrategy;
    int m_interval;
    int m_moveTime;
    bool m_moving;
    int m_timeSinceLastChange;
    int m_timeMoved;
public:
    IntervalMovement(UnderlyingStrategy strategy, int interval, int moveTime)
        : underlyingStrategy(std::move(strategy)), m_interval(interval), m_moveTime(moveTime),
          m_moving(true), m_timeSinceLastChange(0) {}

    std::pair<int, int> move(int currentPosition, int anchorPosition, int deltaTime) {
        m_timeSinceLastChange += deltaTime;
        if (m_moving) {
            m_timeMoved += deltaTime;
            if (m_timeMoved >= m_moveTime) {
                // Switch to stationary phase
                m_moving = false;
                m_timeMoved = 0;
                m_timeSinceLastChange = 0;  // reset the time since last change
            } else {
                return std::visit(
                    [currentPosition, anchorPosition, deltaTime](auto& strategy) {
                        return strategy.move(currentPosition, anchorPosition, deltaTime);
                    },
                    underlyingStrategy
                );
            }
        } else if (m_timeSinceLastChange >= m_interval) {
            // Switch to moving phase
            m_moving = true;
            m_timeSinceLastChange = 0;  // reset the time since last change
        }

        return {currentPosition, anchorPosition};  // Return current position if not moving
    }
};

}

}



