#include "MenuView.h"

namespace Game {
namespace Core {

MenuView::MenuView(QRect screenGeometry, QWidget *parent)
    : QGraphicsView(parent), m_screenGeometry(screenGeometry) {
  setupScene();
}

void MenuView::createMenuItem(const QString &text, MenuAction menuAction,
                              QColor color, int fontSize, const QPointF &pos,
                              QVariant payload) {
  MenuTextItem *item = new MenuTextItem(text, color, menuAction);
  item->setPayload(payload);
  QFont font;
  font.setPointSize(fontSize);
  item->setFont(font);
  item->setPos(pos);

  connect(item, &MenuTextItem::clicked, this, &MenuView::onMenuItemClicked);
  scene()->addItem(item);
}

void MenuView::setupScene() {
  QGraphicsScene *scene = new QGraphicsScene(this);
  scene->setBackgroundBrush(QColor(0, 0, 0));
  setScene(scene);
  setSceneRect(m_screenGeometry);
}

void MenuView::onMenuItemClicked() {
  MenuTextItem *item = qobject_cast<MenuTextItem *>(sender());
  if (!item)
    return;

  MenuAction menuAction = item->menuAction();
  switch (menuAction) {
  case MenuAction::NEW_GAME:
    emit newGameSelected();
    break;
  case MenuAction::RESUME_GAME:
    emit resumeGameSelected();
    break;
  case MenuAction::LEVEL_SELECTOR:
    emit levelSelectorSelected();
    break;
  case MenuAction::OPTIONS:
    break;
  case MenuAction::SELECT_LEVEL:
    if (!item->payload().isNull()) {
      LevelInfo levelInfo = qvariant_cast<LevelInfo>(item->payload());
      emit levelSelected(levelInfo);
    }
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
