#ifndef GAME_CORE_MAINMENUVIEW_H
#define GAME_CORE_MAINMENUVIEW_H

#include "MenuView.h"
#include <QGraphicsView>
#include <QObject>

namespace Game {
namespace Core {

class MainMenuView : public MenuView {
  Q_OBJECT
public:
  MainMenuView(QRect screenGeometry, QWidget *parent = nullptr);
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_MAINMENUVIEW_H
