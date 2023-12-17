#ifndef GAME_CORE_PAUSEMENUVIEW_H
#define GAME_CORE_PAUSEMENUVIEW_H

#include "MenuView.h"

namespace Game {
namespace Core {

class PauseMenuView : public MenuView {
  Q_OBJECT
public:
  PauseMenuView(QRect screenGeometry, QWidget *parent = nullptr);
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_PAUSEMENUVIEW_H
