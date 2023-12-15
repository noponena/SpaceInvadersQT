#ifndef GAME_CORE_MAINMENUVIEW_H
#define GAME_CORE_MAINMENUVIEW_H

#include "Game/Core/MenuTextItem.h"
#include <QGraphicsView>
#include <QObject>

namespace Game {
namespace Core {

class MainMenuView : public QGraphicsView {
  Q_OBJECT
public:
  MainMenuView(QWidget *parent = nullptr);

private:
  void createMenuItem(const QString &text, MenuAction menuAction, QColor color,
                      int fontSize, const QPointF &pos);
private slots:
  void onMenuItemClicked();
signals:
  void windowClosed();
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_MAINMENUVIEW_H
