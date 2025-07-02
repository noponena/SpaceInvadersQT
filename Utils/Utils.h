#ifndef UTILS_H
#define UTILS_H

#include "GameObjects/GameObject.h"
#include <filesystem>
#include <string>
#include <unordered_map>

enum class GameDataType {
  ROOT,
  BLUEPRINT,
  LEVEL,
};

namespace Utils {

float lerp(float a, float b, float t);
int randi(int lowerBound, int upperBound);
bool probabilityCheck(float probability);
std::string getTimestampStr();
std::filesystem::path getDataFolderPath(GameDataType type);
QColor getRandomColor();

inline GameObjects::ConcreteType
concreteTypeFromString(const std::string &typeStr) {
  static const std::unordered_map<std::string, GameObjects::ConcreteType>
      lookup = {
          {"PLAYER_SHIP", GameObjects::ConcreteType::PLAYER_SHIP},
          {"ENEMY_SHIP", GameObjects::ConcreteType::ENEMY_SHIP},
          {"PROJECTILE", GameObjects::ConcreteType::PROJECTILE},
          {"VORTEX", GameObjects::ConcreteType::VORTEX},
          {"WAVE_OF_DESTRUCTION",
           GameObjects::ConcreteType::WAVE_OF_DESTRUCTION},
          {"STELLAR_COIN", GameObjects::ConcreteType::STELLAR_COIN},
          {"HEALTH", GameObjects::ConcreteType::HEALTH},
      };
  auto it = lookup.find(typeStr);
  if (it == lookup.end())
    throw std::invalid_argument("Unknown ConcreteType: " + typeStr);
  return it->second;
}

inline std::string concreteTypeToString(const GameObjects::ConcreteType type) {
  switch (type) {
  case GameObjects::ConcreteType::PLAYER_SHIP:
    return "PLAYER_SHIP";
  case GameObjects::ConcreteType::ENEMY_SHIP:
    return "ENEMY_SHIP";
  case GameObjects::ConcreteType::PROJECTILE:
    return "PROJECTILE";
  case GameObjects::ConcreteType::VORTEX:
    return "VORTEX";
  case GameObjects::ConcreteType::WAVE_OF_DESTRUCTION:
    return "WAVE_OF_DESTRUCTION";
  case GameObjects::ConcreteType::STELLAR_COIN:
    return "STELLAR_COIN";
  case GameObjects::ConcreteType::HEALTH:
    return "HEALTH";
  default:
    return "UNKNOWN";
  }
}

} // namespace Utils

#endif // UTILS_H
