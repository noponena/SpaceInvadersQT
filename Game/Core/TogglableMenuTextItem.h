#ifndef GAME_CORE_TOGGLABLEMENUTEXTITEM_H
#define GAME_CORE_TOGGLABLEMENUTEXTITEM_H

#include "MenuTextItem.h"

namespace Game {
namespace Core {

class TogglableMenuTextItem : public MenuTextItem {
  Q_OBJECT
public:
  TogglableMenuTextItem(QString text, QColor color, MenuAction menuAction,
                        QGraphicsItem *parent = nullptr);
  void setSelected(bool selected);

private:
  bool m_isSelected;
  QColor m_selectedColor;
  void updateVisuals();

  // QGraphicsItem interface
protected:
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_TOGGLABLEMENUTEXTITEM_H
