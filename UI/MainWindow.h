#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game/Core/GameRunnerView.h"
#include "Game/Core/MainMenuView.h"
#include "Game/Core/PauseMenuView.h"
#include <QMainWindow>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  Game::Core::MainMenuView *m_mainMenuView;
  Game::Core::PauseMenuView *m_pauseMenuView;
  Game::Core::GameRunnerView *m_gameRunnerView;
  QStackedWidget *m_stackedWidget;

private slots:
  void newGame();
  void resumeGame();
  void onGamePaused();
  void onWindowClosed();
  void bringToForeground();
  void adjustMainMenuSize();
  void adjustGameRunnerSize();
};
#endif // MAINWINDOW_H
