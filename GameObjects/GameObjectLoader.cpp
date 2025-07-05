#include "GameObjectLoader.h"
#include "Game/Movement/MovementStrategyLoader.h"
#include "Utils/Utils.h"

namespace GameObjects {
GameObjects::GameObjectBlueprint
GameObjectLoader::loadFromFile(const std::filesystem::path &path) {
  const std::string strPath = path.string();
  qDebug() << "[GameObjectLoader] Loading GameObject from file:"
           << QString::fromStdString(strPath);
  YAML::Node doc = YAML::LoadFile(strPath);
  auto bp = loadFromYaml(doc);
  qDebug() << "[GameObjectLoader] Loaded GameObjectBlueprint with name:"
           << QString::fromStdString(bp.name) << ", type:"
           << QString::fromStdString(doc["type"].as<std::string>());
  return bp;
}

std::vector<GameObjects::GameObjectBlueprint>
GameObjectLoader::loadMultipleFromFile(const std::string &path) {
  qDebug() << "[GameObjectLoader] Loading multiple GameObjects from file:"
           << QString::fromStdString(path);
  YAML::Node doc = YAML::LoadFile(path);
  std::vector<GameObjects::GameObjectBlueprint> result;
  int idx = 0;
  for (const auto &node : doc) {
    auto bp = loadFromYaml(node);
    qDebug() << "[GameObjectLoader] Loaded GameObjectBlueprint #" << idx++
             << "name:" << QString::fromStdString(bp.name) << ", type:"
             << QString::fromStdString(node["type"].as<std::string>());
    result.push_back(std::move(bp));
  }
  return result;
}

GameObjects::GameObjectBlueprint
GameObjectLoader::loadFromYaml(const YAML::Node &node) {
  GameObjects::GameObjectBlueprint bp;
  bp.name = node["name"].as<std::string>();
  bp.type = Utils::concreteTypeFromString(node["type"].as<std::string>());
  qDebug() << "[GameObjectLoader] Parsing blueprint: name ="
           << QString::fromStdString(bp.name) << ", type ="
           << QString::fromStdString(node["type"].as<std::string>());
  if (node["damage"]) {
    bp.damage = node["damage"].as<float>();
    qDebug() << "[GameObjectLoader] ...damage =" << bp.damage;
  }
  if (node["health"]) {
    bp.maxHealth = node["health"].as<float>();
    qDebug() << "[GameObjectLoader] ...maxHealth =" << bp.maxHealth;
  }
  if (node["movement"]) {
    const std::filesystem::path filePath = node["movement"].as<std::string>();
    const std::filesystem::path dirPath =
        Utils::getDataFolderPath(GameDataType::BLUEPRINT) / "movement";
    const std::filesystem::path fullPath = dirPath / filePath;
    qDebug() << "[GameObjectLoader] ...loading movement from:"
             << QString::fromStdString(fullPath.string());
    bp.movement =
        Game::Movement::MovementStrategyLoader::loadFromFile(fullPath);
  }
  return bp;
}

} // namespace GameObjects
