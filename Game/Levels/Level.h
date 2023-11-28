#ifndef GAME_LEVELS_LEVEL_H
#define GAME_LEVELS_LEVEL_H

namespace Game {
namespace Levels {

class Level {
public:
  Level();
  int number;
  int enemySpawnIntervalMs;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_LEVEL_H
