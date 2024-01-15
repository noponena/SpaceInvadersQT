#ifndef GAME_CORE_MENUVIEW_H
#define GAME_CORE_MENUVIEW_H

#include "Game/Core/MenuTextItem.h"
#include "Game/Levels/Level.h"
#include <QGraphicsView>

namespace Game {
namespace Core {

class MenuView : public QGraphicsView {
  Q_OBJECT
public:
  MenuView(QRect screenGeometry, QWidget *parent = nullptr);

protected:
  QRect m_screenGeometry;
  virtual void createMenuItem(const QString &text, MenuAction menuAction,
                              QColor color, int fontSize, const QPointF &pos,
                              QVariant payload = QVariant());

private:
  void setupScene();
protected slots:
  virtual void onMenuItemClicked();
signals:
  void newGameSelected();
  void levelSelectorSelected();
  void levelSelected(Game::Levels::Level level);
  void startLevelSelected();
  void quitLevelSelected();
  void backToMainMenuSelected();
  void resumeGameSelected();
  void windowClosed();
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_MENUVIEW_H
