#include "UI/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  qRegisterMetaType<Game::Levels::Level>();
  w.show();
  return a.exec();
}
