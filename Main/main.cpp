#include "UI/MainWindow.h"
#include "Graphics/PixmapRegistry.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  PixmapRegistry::instance().preloadAll();
  QApplication a(argc, argv);
  MainWindow w;
  qRegisterMetaType<Game::Levels::Level>();
  w.show();
  return a.exec();
}
