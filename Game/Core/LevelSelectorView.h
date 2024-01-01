#ifndef GAME_CORE_LEVELSELECTORVIEW_H
#define GAME_CORE_LEVELSELECTORVIEW_H

#include "Game/Core/TogglableMenuTextItem.h"
#include "Game/Levels/Level.h"
#include "MenuView.h"

namespace Game {
namespace Core {

class LevelSelectorView : public MenuView {
  Q_OBJECT
public:
  LevelSelectorView(QRect screenGeometry, QWidget *parent = nullptr);
  void setLevelData(std::map<int, Levels::Level> levels);

private:
  Levels::Level m_currentLevel;
  bool m_levelAllowedToStart;
  TogglableMenuTextItem *m_selectedItem = nullptr;
private slots:
  void onLevelSelected(Levels::Level level);
  void onStartLevelSelected();
signals:
  void levelStarted(Levels::Level level);

  // MenuView interface
protected slots:
  void onMenuItemClicked() override;

  // MenuView interface
protected:
  void createMenuItem(const QString &text, MenuAction menuAction, QColor color,
                      int fontSize, const QPointF &pos,
                      QVariant payload = QVariant()) override;
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_LEVELSELECTORVIEW_H
