#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cmath>
#include <variant>

namespace Game {

namespace AxisMovementStrategies {

class LinearMovement {
    float m_speed;
    int m_direction;
public:
    LinearMovement(float speed, int direction = 1) : m_speed(speed), m_direction(direction) {}
    int move(int currentPosition, int deltaTime) {
        return currentPosition + static_cast<int>(m_speed * deltaTime) * m_direction;
    }
};

class SinusoidMovement {
    float m_amplitude, m_frequency, m_accumulatedTime, m_phase;
    int m_direction;
public:
    SinusoidMovement(float amplitude, float frequency, float phase, int direction = 1)
        : m_amplitude(amplitude), m_frequency(frequency), m_accumulatedTime(0), m_phase(phase), m_direction(direction) {}

    int move(int currentPosition, int deltaTime) {
        m_accumulatedTime += deltaTime / 1000.0f;
        return currentPosition + static_cast<int>(m_amplitude * std::sin(2 * M_PI * m_frequency * m_accumulatedTime + m_phase * M_PI)) * m_direction;
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

    int move(int currentPosition, int deltaTime) {
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
                    [currentPosition, deltaTime](auto& strategy) {
                        return strategy.move(currentPosition, deltaTime);
                    },
                    underlyingStrategy
                );
            }
        } else if (m_timeSinceLastChange >= m_interval) {
            // Switch to moving phase
            m_moving = true;
            m_timeSinceLastChange = 0;  // reset the time since last change
        }

        return currentPosition;  // Return current position if not moving
    }
};
}

}



