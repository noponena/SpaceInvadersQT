#ifndef GAME_CORE_MENUTEXTITEM_H
#define GAME_CORE_MENUTEXTITEM_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QVariant>

namespace Game {
namespace Core {

enum class MenuAction {
  NEW_GAME,
  RESUME_GAME,
  LEVEL_SELECTOR,
  OPTIONS,
  QUIT,
  SELECT_LEVEL
};

class MenuTextItem : public QGraphicsTextItem {
  Q_OBJECT
public:
  MenuTextItem(QString text, QColor color, MenuAction menuAction,
               QGraphicsItem *parent = nullptr);
  void setPayload(const QVariant &payload);
  MenuAction menuAction() const;
  QVariant payload() const;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
  MenuAction m_menuAction;
  QColor m_textColor;
  QColor m_textHoverColor;
  QVariant m_payload;
signals:
  void clicked();
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_MENUTEXTITEM_H
