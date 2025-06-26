#include "Graphics/PixmapRegistry.h"
#include "UI/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  PixmapRegistry::instance().preloadAll();
  MainWindow w;
  qRegisterMetaType<Game::Levels::Level>();
  w.show();
  return a.exec();
}
