#include "MainMenuView.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

namespace Game {
namespace Core {

MainMenuView::MainMenuView(QRect screenGeometry, QWidget *parent)
    : MenuView(screenGeometry, parent) {
  QColor color = QColor(255, 255, 255);
  // createMenuItem("New Game", MenuAction::NEW_GAME, color, 100,
  //                QPointF(100, 150));
  createMenuItem("Level Selector", MenuAction::LEVEL_SELECTOR, color, 100,
                 QPointF(100, 150));
  createMenuItem("Options", MenuAction::OPTIONS, color, 100, QPointF(100, 300));
  createMenuItem("Benchmark", MenuAction::BENCHMARK, color, 100,
                 QPointF(100, 450));
  createMenuItem("Quit", MenuAction::QUIT, color, 100, QPointF(100, 600));
}

} // namespace Core
} // namespace Game
