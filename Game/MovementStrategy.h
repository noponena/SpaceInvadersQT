#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <vector>
#include <tuple>
#include <variant>
#include "AxisMovementStrategy.h"  // Include or forward-declare your movement strategy classes

namespace Game {
namespace MovementStrategies {

using AxisMovementStrategy =
std::variant
<
AxisMovementStrategies::LinearMovement,
AxisMovementStrategies::SinusoidMovement,
AxisMovementStrategies::IntervalMovement
>;

class MovementStrategy {

public:
    MovementStrategy() {
        m_xAxisMovementStrategies.reserve(m_maxStrategies);
        m_yAxisMovementStrategies.reserve(m_maxStrategies);
    }

    std::tuple<int, int> move(const std::tuple<int, int>& xyPos, int deltaTime) {
        int x = std::get<0>(xyPos);
        int y = std::get<1>(xyPos);
        for (auto& strategy : m_xAxisMovementStrategies)
        {
            x = std::visit([&](auto&& arg) { return arg.move(x, deltaTime); }, strategy);
        }
        for (auto& strategy : m_yAxisMovementStrategies)
        {
            y = std::visit([&](auto&& arg) { return arg.move(y, deltaTime); }, strategy);
        }
        return std::make_tuple(x, y);
    }

    void clear() {
        this->m_xAxisMovementStrategies.clear();
        this->m_yAxisMovementStrategies.clear();
    }

protected:
    std::vector<AxisMovementStrategy> m_xAxisMovementStrategies;
    std::vector<AxisMovementStrategy> m_yAxisMovementStrategies;
private:
    int m_maxStrategies = 10;
};

class PlayerProjectileMovementStrategy : public MovementStrategy {

public:
    PlayerProjectileMovementStrategy(float speed = 1, int direction = 1) {
        m_yAxisMovementStrategies.push_back(AxisMovementStrategies::LinearMovement(speed, direction));
    }
};

class CircularMovementStrategy : public MovementStrategy {

public:
    CircularMovementStrategy(float amplitude, float frequency, int direction = 1) {
        m_xAxisMovementStrategies.push_back(AxisMovementStrategies::SinusoidMovement(amplitude, frequency, 0, direction));
        m_yAxisMovementStrategies.push_back(AxisMovementStrategies::SinusoidMovement(amplitude, frequency, 0.5, direction));
    }
};


}
}

#endif // MOVEMENTSTRATEGY_H
