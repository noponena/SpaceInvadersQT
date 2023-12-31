#ifndef GAME_CORE_MENUVIEW_H
#define GAME_CORE_MENUVIEW_H

#include "Game/Core/MenuTextItem.h"
#include <QGraphicsView>

namespace Game {
namespace Core {

struct LevelInfo {
  int levelNumber;
  QString name;
};

class MenuView : public QGraphicsView {
  Q_OBJECT
public:
  MenuView(QRect screenGeometry, QWidget *parent = nullptr);

protected:
  QRect m_screenGeometry;
  void createMenuItem(const QString &text, MenuAction menuAction, QColor color,
                      int fontSize, const QPointF &pos,
                      QVariant payload = QVariant());

private:
  void setupScene();
private slots:
  void onMenuItemClicked();
signals:
  void newGameSelected();
  void levelSelectorSelected();
  void levelSelected(LevelInfo levelInfo);
  void resumeGameSelected();
  void windowClosed();
};

} // namespace Core
} // namespace Game

Q_DECLARE_METATYPE(Game::Core::LevelInfo)

#endif // GAME_CORE_MENUVIEW_H
