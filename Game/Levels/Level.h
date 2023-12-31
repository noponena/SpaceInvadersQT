#ifndef GAME_LEVELS_LEVEL_H
#define GAME_LEVELS_LEVEL_H

#include "SpawnEvent.h"

namespace Game {
namespace Levels {

class Level {
public:
  Level();
  int levelNumber;
  int enemyLimit;
  std::string name;
  std::string description;
  std::vector<SpawnEvent> spawnEvents;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_LEVEL_H
