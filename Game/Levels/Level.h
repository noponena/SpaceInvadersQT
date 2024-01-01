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

Q_DECLARE_METATYPE(Game::Levels::Level)

#endif // GAME_LEVELS_LEVEL_H
