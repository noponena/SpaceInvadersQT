#pragma once

#include "GameObjectBlueprint.h"
#include <filesystem>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace GameObjects {
class GameObjectLoader {
public:
  /**
   * @brief Loads a GameObjectBlueprint from a single YAML file.
   * @param path Path to the YAML file.
   * @return GameObjectBlueprint instance parsed from file.
   * @throws std::runtime_error if loading or parsing fails.
   */
  static GameObjects::GameObjectBlueprint
  loadFromFile(const std::filesystem::path &path);

  /**
   * @brief Loads multiple GameObjectBlueprints from a YAML file containing a
   * list of objects.
   * @param path Path to the YAML file.
   * @return Vector of GameObjectBlueprints.
   * @throws std::runtime_error if loading or parsing fails.
   */
  static std::vector<GameObjects::GameObjectBlueprint>
  loadMultipleFromFile(const std::string &path);

  /**
   * @brief Loads a GameObjectBlueprint from a YAML node.
   * @param node YAML node.
   * @return GameObjectBlueprint.
   * @throws std::runtime_error if parsing fails.
   */
  static GameObjects::GameObjectBlueprint loadFromYaml(const YAML::Node &node);
};
} // namespace GameObjects
