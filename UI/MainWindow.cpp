#include "MainWindow.h"
#include "Game/Core/GameRunnerView.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Central widget that will hold the layout
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(centralWidget);
  layout->setSpacing(0); // No space between the widgets in the layout
  layout->setContentsMargins(0, 0, 0, 0); // No margins around the layout
  setContentsMargins(0, 0, 0, 0);
  statusBar()->hide();

  // Create the game runner scene and view
  m_gameRunnerView = new Game::Core::GameRunnerView(centralWidget);
  m_gameRunnerView->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);

  // Add the views to the layout
  layout->addWidget(m_gameRunnerView);

  // Set central widget of the main window
  setCentralWidget(centralWidget);

  setStyleSheet("border:0px");

  QTimer::singleShot(0, m_gameRunnerView,
                     [this]() { m_gameRunnerView->startGame(); });
  QTimer::singleShot(0, this, SLOT(bringToForeground()));
  connect(m_gameRunnerView, &Game::Core::GameRunnerView::windowClosed, this,
          &MainWindow::onWindowClosed);

  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  resize(screenGeometry.width(), screenGeometry.height());
  QApplication::setOverrideCursor(Qt::BlankCursor);

  setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  showFullScreen();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event); // Call base class handler
  adjustGameRunnerSize();
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

void MainWindow::onWindowClosed() { QCoreApplication::quit(); }
