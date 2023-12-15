#include "MenuTextItem.h"

namespace Game {
namespace Core {

MenuTextItem::MenuTextItem(QString text, QColor color, MenuAction menuAction,
                           QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent), m_menuAction(menuAction) {
  setAcceptHoverEvents(true);
  setDefaultTextColor(color);
  m_textHoverColor = Qt::yellow;
  m_textColor = color;
}

void MenuTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  Q_UNUSED(event);
  emit clicked();
}

void MenuTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  Q_UNUSED(event);
  setDefaultTextColor(m_textHoverColor);
}

void MenuTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  Q_UNUSED(event);
  setDefaultTextColor(m_textColor);
}

MenuAction MenuTextItem::menuAction() const { return m_menuAction; }

} // namespace Core
} // namespace Game
