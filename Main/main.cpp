#include "UI/MainWindow.h"
// #include "Game/Levels/LevelLoader.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
  // Game::Levels::LevelLoader levelLoader(1000, 1000);
  // std::vector<Game::Levels::Level> levels = levelLoader.loadLevels();
  // qDebug() << "level count:" << levels.size();
  // return 0;
}
