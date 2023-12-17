#include "PauseMenuView.h"

namespace Game {
namespace Core {

PauseMenuView::PauseMenuView(QRect screenGeometry, QWidget *parent)
    : MenuView(screenGeometry, parent) {
  QColor color = QColor(255, 255, 255);
  createMenuItem("Resume Game", MenuAction::RESUME_GAME, color, 100,
                 QPointF(100, 150));
  createMenuItem("Level Selector", MenuAction::LEVEL_SELECTOR, color, 100,
                 QPointF(100, 300));
  createMenuItem("Options", MenuAction::OPTIONS, color, 100, QPointF(100, 450));
  createMenuItem("Quit", MenuAction::QUIT, color, 100, QPointF(100, 600));
}

} // namespace Core
} // namespace Game
