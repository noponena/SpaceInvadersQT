#include "MainWindow.h"
#include "Game/Core/GameRunner.h"
#include "ui_MainWindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QTimer::singleShot(0, ui->gameRunner, &Game::Core::GameRunner::startGame);
  QTimer::singleShot(0, this, SLOT(bringToForeground()));
  // QTimer::singleShot(0, this, SLOT(adjustGameRunnerSize()));
  connect(ui->gameRunner, &Game::Core::GameRunner::windowClosed, this,
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
  if (ui->gameRunner) {
    // Adjust the scene rect to the new size
    QRectF newRect(0, 0, width(), height());
    ui->gameRunner->scene()->setSceneRect(newRect);
    ui->gameRunner->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->gameRunner->showFullScreen();

    // No need to set styles or window flags here
  }
}

void MainWindow::bringToForeground() {
  ui->gameRunner->activateWindow();
  ui->gameRunner->raise();
  ui->gameRunner->setFocus();
}

void MainWindow::onWindowClosed() { QCoreApplication::quit(); }
