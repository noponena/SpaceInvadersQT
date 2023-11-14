#include "MainWindow.h"
#include "Game/GameRunner.h"
#include "ui_MainWindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QTimer::singleShot(0, ui->gameRunner, &Game::GameRunner::startGame);
  QTimer::singleShot(0, this, SLOT(bringToForeground()));
  connect(ui->gameRunner, &Game::GameRunner::windowClosed, this, &MainWindow::onWindowClosed);
  this->setStyleSheet("background-color: black;");

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event); // Call base class handler
  if (ui->gameRunner) {

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    this->resize(screenGeometry.width(), screenGeometry.height());

    QRectF newRect(0, 0, this->width(), this->height());
    ui->gameRunner->scene()->setSceneRect(newRect);

    this->setContentsMargins(0, 0, 0, 0);
    this->setStyleSheet("border:0px");
    //this->ui->gameRunner->setStyleSheet("border:0px");
    this->ui->gameRunner->setContentsMargins(0, 0, 0, 0);
    this->ui->gameRunner->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->ui->gameRunner->showFullScreen();
  }
}

void MainWindow::bringToForeground()
{
  this->ui->gameRunner->activateWindow();
  this->ui->gameRunner->raise();
  this->ui->gameRunner->setFocus();
}

void MainWindow::onWindowClosed()
{
  QCoreApplication::quit();
}
