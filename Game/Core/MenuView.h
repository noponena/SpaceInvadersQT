#ifndef GAME_CORE_MENUVIEW_H
#define GAME_CORE_MENUVIEW_H

#include "Game/Core/MenuTextItem.h"
#include <QGraphicsView>

namespace Game {
namespace Core {

class MenuView : public QGraphicsView {
  Q_OBJECT
public:
  MenuView(QRect screenGeometry, QWidget *parent = nullptr);

protected:
  QRect m_screenGeometry;
  void createMenuItem(const QString &text, MenuAction menuAction, QColor color,
                      int fontSize, const QPointF &pos);

private:
  void setupScene();
private slots:
  void onMenuItemClicked();
signals:
  void newGameSelected();
  void resumeGameSelected();
  void windowClosed();
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_MENUVIEW_H
