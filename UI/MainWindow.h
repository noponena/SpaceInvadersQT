#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game/Core/GameRunnerView.h"
#include "Game/Core/LevelSelectorView.h"
#include "Game/Core/MainMenuView.h"
#include "Game/Core/PauseMenuView.h"
#include "Game/Core/BenchmarkPromptView.h"
#include "Game/Levels/LevelLoader.h"
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
  Game::Levels::LevelLoader m_levelLoader;
  std::map<int, Game::Levels::Level> m_levels;
  Game::Core::MainMenuView *m_mainMenuView;
  Game::Core::LevelSelectorView *m_levelSelectorView;
  Game::Core::PauseMenuView *m_pauseMenuView;
  Game::Core::BenchmarkPromptView *m_benchmarkPromptView;
  Game::Core::GameRunnerView *m_gameRunnerView;
  QStackedWidget *m_stackedWidget;
  int m_currentLevelNumber;

private slots:
  void newGame();
  void resumeGame();
  void levelSelector();
  void onGamePaused();
  void onLevelQuit();
  void onWindowClosed();
  void onLevelStarted(Game::Levels::Level level);
  void onBackToMainMenuSelected();
  void onBenchmarkSelected();
  void startBenchmark();
  void bringToForeground();
  void adjustMainMenuSize();
  void adjustGameRunnerSize();
};
#endif // MAINWINDOW_H
