#include "TogglableMenuTextItem.h"

namespace Game {
namespace Core {

TogglableMenuTextItem::TogglableMenuTextItem(QString text, QColor color,
                                             MenuAction menuAction,
                                             QGraphicsItem *parent)
    : MenuTextItem(text, color, menuAction, parent), m_isSelected(false),
      m_selectedColor(Qt::cyan) {}

void TogglableMenuTextItem::setSelected(bool selected) {
  m_isSelected = selected;
  updateVisuals();
}

void TogglableMenuTextItem::updateVisuals() {
  if (m_isSelected) {
    setDefaultTextColor(m_selectedColor);
  } else {
    setDefaultTextColor(m_textColor);
  }
}

void TogglableMenuTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  if (!m_isSelected)
    MenuTextItem::hoverEnterEvent(event);
}

void TogglableMenuTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
  if (!m_isSelected)
    MenuTextItem::hoverLeaveEvent(event);
}

} // namespace Core
} // namespace Game
