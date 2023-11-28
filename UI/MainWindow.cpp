#include "MainWindow.h"
#include "Game/Core/GameRunnerScene.h"
#include "ui_MainWindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QTimer::singleShot(0, ui->gameRunnerScene,
                     &Game::Core::GameRunnerScene::startGame);
  QTimer::singleShot(0, this, SLOT(bringToForeground()));
  // QTimer::singleShot(0, this, SLOT(adjustGameRunnerSize()));
  connect(ui->gameRunnerScene, &Game::Core::GameRunnerScene::windowClosed, this,
          &MainWindow::onWindowClosed);

  setStyleSheet("background-color: black;");
  setStyleSheet("border:0px");
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  resize(screenGeometry.width(), screenGeometry.height());
  QApplication::setOverrideCursor(Qt::BlankCursor);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event); // Call base class handler
  adjustGameRunnerSize();
}

void MainWindow::adjustGameRunnerSize() {
  if (ui->gameRunnerScene) {
    // Adjust the scene rect to the new size
    QRectF newRect(0, 0, width(), height());
    ui->gameRunnerScene->scene()->setSceneRect(newRect);
    ui->gameRunnerScene->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->gameRunnerScene->showFullScreen();

    // No need to set styles or window flags here
  }
}

void MainWindow::bringToForeground() {
  ui->gameRunnerScene->activateWindow();
  ui->gameRunnerScene->raise();
  ui->gameRunnerScene->setFocus();
}

void MainWindow::onWindowClosed() { QCoreApplication::quit(); }
