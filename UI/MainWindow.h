#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game/Core/GameRunnerView.h"
#include "Game/Core/MainMenuView.h"
#include <QMainWindow>

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

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  Ui::MainWindow *ui;
  Game::Core::MainMenuView *m_mainMenuView;
  Game::Core::GameRunnerView *m_gameRunnerView;

private slots:
  void onWindowClosed();
  void bringToForeground();
  void adjustMainMenuSize();
  void adjustGameRunnerSize();
};
#endif // MAINWINDOW_H
