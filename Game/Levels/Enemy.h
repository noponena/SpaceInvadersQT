#ifndef GAME_LEVELS_ENEMY_H
#define GAME_LEVELS_ENEMY_H

#include <QPoint>
#include <string>

namespace Game {
namespace Levels {

class Enemy {
public:
  Enemy();
  std::string type;
  QPoint position;
  std::string behavior;
  int intervalMs;
  int count;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_ENEMY_H
