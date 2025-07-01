#pragma once

#include "MovementStrategy.h"
#include <filesystem>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Movement {

/**
 * @brief Loads MovementStrategy objects from YAML files.
 */
class MovementStrategyLoader {
public:
  /**
   * @brief Loads a movement strategy from a YAML file.
   * @param path Path to the YAML file.
   * @return Loaded MovementStrategy.
   * @throws std::runtime_error if loading or parsing fails.
   */
  static MovementStrategy loadFromFile(const std::filesystem::path &path);

  /**
   * @brief Loads a movement strategy from a YAML node (for in-memory loading).
   * @param node YAML node containing the strategy definition.
   * @return Loaded MovementStrategy.
   * @throws std::runtime_error if parsing fails.
   */
  static MovementStrategy loadFromYaml(const YAML::Node &node);

private:
  static void loadAxis(const YAML::Node &axisNode,
                       std::vector<AxisMovementStrategy> &out);
  static AxisMovementStrategy parseStep(const YAML::Node &node);
  static IntervalMovement parseInterval(const YAML::Node &node, bool isXAxis);
};

} // namespace Movement
} // namespace Game
