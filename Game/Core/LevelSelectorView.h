#ifndef GAME_CORE_LEVELSELECTORVIEW_H
#define GAME_CORE_LEVELSELECTORVIEW_H

#include "Game/Levels/Level.h"
#include "MenuView.h"

namespace Game {
namespace Core {

class LevelSelectorView : public MenuView {
public:
  LevelSelectorView(QRect screenGeometry, QWidget *parent = nullptr);
  void setLevelData(std::unordered_map<int, Levels::Level> levels);
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_LEVELSELECTORVIEW_H
