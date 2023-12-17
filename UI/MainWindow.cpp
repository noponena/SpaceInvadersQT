#include "MainWindow.h"
#include "Game/Core/GameRunnerView.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setContentsMargins(0, 0, 0, 0);
  statusBar()->hide();

  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();

  // Create the game runner scene and view
  m_gameRunnerView = new Game::Core::GameRunnerView(screenGeometry);
  m_gameRunnerView->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);

  m_mainMenuView = new Game::Core::MainMenuView(screenGeometry);
  m_mainMenuView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  m_pauseMenuView = new Game::Core::PauseMenuView(screenGeometry);
  m_pauseMenuView->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Expanding);

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->addWidget(m_mainMenuView);
  m_stackedWidget->addWidget(m_pauseMenuView);
  m_stackedWidget->addWidget(m_gameRunnerView);

  // Set central widget of the main window
  setCentralWidget(m_stackedWidget);

  setStyleSheet("border:0px");

  connect(m_gameRunnerView, &Game::Core::GameRunnerView::gamePaused, this,
          &MainWindow::onGamePaused);
  connect(m_mainMenuView, &Game::Core::MainMenuView::windowClosed, this,
          &MainWindow::onWindowClosed);
  connect(m_mainMenuView, &Game::Core::MainMenuView::newGameSelected, this,
          &MainWindow::newGame);

  connect(m_pauseMenuView, &Game::Core::PauseMenuView::resumeGameSelected, this,
          &MainWindow::resumeGame);
  connect(m_pauseMenuView, &Game::Core::PauseMenuView::windowClosed, this,
          &MainWindow::onWindowClosed);

  m_stackedWidget->setCurrentWidget(m_mainMenuView);

  resize(screenGeometry.width(), screenGeometry.height());

  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  showFullScreen();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::newGame() {
  QApplication::setOverrideCursor(Qt::BlankCursor);
  m_stackedWidget->setCurrentWidget(m_gameRunnerView);
  m_gameRunnerView->startGame();
}

void MainWindow::resumeGame() {
  QApplication::setOverrideCursor(Qt::BlankCursor);
  m_stackedWidget->setCurrentWidget(m_gameRunnerView);
  m_gameRunnerView->resumeGame();
}

void MainWindow::onGamePaused() {
  QApplication::restoreOverrideCursor();
  m_stackedWidget->setCurrentWidget(m_pauseMenuView);
}

void MainWindow::adjustGameRunnerSize() {
  if (m_gameRunnerView) {
    QRectF newRect(0, 0, width(), height());
    m_gameRunnerView->scene()->setSceneRect(newRect);
  }
}

void MainWindow::bringToForeground() {
  m_gameRunnerView->activateWindow();
  m_gameRunnerView->raise();
  m_gameRunnerView->setFocus();
}

void MainWindow::adjustMainMenuSize() {
  if (m_mainMenuView) {
    QRectF newRect(0, 0, width(), height());
    m_mainMenuView->scene()->setSceneRect(newRect);
  }
}

void MainWindow::onWindowClosed() { QCoreApplication::quit(); }
