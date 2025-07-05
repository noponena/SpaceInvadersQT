#include "UI/MainWindow.h"
#include "Utils/CustomMessageHandler.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qInstallMessageHandler(customMessageHandler);
  MainWindow w;
  qRegisterMetaType<Game::Levels::Level>();
  w.show();
  return a.exec();
}
