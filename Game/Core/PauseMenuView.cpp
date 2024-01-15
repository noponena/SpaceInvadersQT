#include "PauseMenuView.h"

namespace Game {
namespace Core {

PauseMenuView::PauseMenuView(QRect screenGeometry, QWidget *parent)
    : MenuView(screenGeometry, parent) {
  QColor color = QColor(255, 255, 255);
  createMenuItem("Resume Game", MenuAction::RESUME_GAME, color, 100,
                 QPointF(100, 150));
  createMenuItem("Options", MenuAction::OPTIONS, color, 100, QPointF(100, 300));
  createMenuItem("Quit to Main Menu", MenuAction::QUIT_LEVEL, color, 100,
                 QPointF(100, 450));
}

} // namespace Core
} // namespace Game
