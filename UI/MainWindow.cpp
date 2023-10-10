#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Game/GameRunner.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(0, ui->gameRunner, &Game::GameRunner::startGame);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);  // Call base class handler
    if (ui->gameRunner) {
        QRectF newRect(0, 0, this->width(), this->height());
        ui->gameRunner->scene()->setSceneRect(newRect);
    }
}

