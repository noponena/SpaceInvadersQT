#include "MainMenuView.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>

namespace Game {
namespace Core {

MainMenuView::MainMenuView(QWidget *parent) : QGraphicsView(parent) {
  QGraphicsScene *scene = new QGraphicsScene(this);
  scene->setBackgroundBrush(QColor(0, 0, 0));
  setScene(scene);
  QColor color = QColor(255, 255, 255);
  createMenuItem("New Game", MenuAction::NEW_GAME, color, 100,
                 QPointF(100, 150));
  createMenuItem("Level Selector", MenuAction::LEVEL_SELECTOR, color, 100,
                 QPointF(100, 300));
  createMenuItem("Options", MenuAction::OPTIONS, color, 100, QPointF(100, 450));
  createMenuItem("Quit", MenuAction::QUIT, color, 100, QPointF(100, 600));
}

void MainMenuView::createMenuItem(const QString &text, MenuAction menuAction,
                                  QColor color, int fontSize,
                                  const QPointF &pos) {
  MenuTextItem *item = new MenuTextItem(text, color, menuAction);
  QFont font;
  font.setPointSize(fontSize);
  item->setFont(font);
  item->setPos(pos);

  connect(item, &MenuTextItem::clicked, this, &MainMenuView::onMenuItemClicked);
  scene()->addItem(item);
}

void MainMenuView::onMenuItemClicked() {
  MenuTextItem *item = qobject_cast<MenuTextItem *>(sender());
  if (!item)
    return;

  MenuAction menuAction = item->menuAction();
  switch (menuAction) {
  case MenuAction::NEW_GAME:
    break;
  case MenuAction::LEVEL_SELECTOR:
    break;
  case MenuAction::OPTIONS:
    break;
  case MenuAction::QUIT:
    emit windowClosed();
    break;
  default:
    break;
  }
}

} // namespace Core
} // namespace Game
