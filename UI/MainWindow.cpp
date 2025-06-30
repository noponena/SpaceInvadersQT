#include "MainWindow.h"
#include "Config/GameContext.h"
#include "Game/Core/GameRunnerView.h"
#include "ui_MainWindow.h"
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_currentLevelNumber(-1) {
  ui->setupUi(this);

  setContentsMargins(0, 0, 0, 0);
  statusBar()->hide();

  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  float screenWidth = screenGeometry.width();
  float screenHeight = screenGeometry.height();

#ifdef NDEBUG
    // Release build: Borderless fullscreen
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    screenWidth = screenGeometry.width();
    screenHeight = screenGeometry.height();
#else
    // Debug build: Windowed, slightly smaller for convenience
    setWindowFlags(Qt::Window);
    screenWidth *= 0.95;
    screenHeight *= 0.95;
#endif

  QRect windowGeometry(0, 0, screenWidth, screenHeight);
  auto gameCtx = Config::GameContext(windowGeometry);

  // Create the game runner scene and view
  m_gameRunnerView = new Game::Core::GameRunnerView(gameCtx);
  m_gameRunnerView->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);

  m_mainMenuView = new Game::Core::MainMenuView(windowGeometry);
  m_mainMenuView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  m_levelSelectorView = new Game::Core::LevelSelectorView(windowGeometry);
  m_levelSelectorView->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);

  m_pauseMenuView = new Game::Core::PauseMenuView(windowGeometry);
  m_pauseMenuView->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Expanding);

  m_benchmarkPromptView = new Game::Core::BenchmarkPromptView(windowGeometry);
  m_benchmarkPromptView->setSizePolicy(QSizePolicy::Expanding,
                                       QSizePolicy::Expanding);

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->addWidget(m_mainMenuView);
  m_stackedWidget->addWidget(m_levelSelectorView);
  m_stackedWidget->addWidget(m_pauseMenuView);
  m_stackedWidget->addWidget(m_benchmarkPromptView);
  m_stackedWidget->addWidget(m_gameRunnerView);

  // Set central widget of the main window
  setCentralWidget(m_stackedWidget);

  setStyleSheet("border:0px");

  connect(m_gameRunnerView, &Game::Core::GameRunnerView::gamePaused, this,
          &MainWindow::onGamePaused);
  connect(m_gameRunnerView, &Game::Core::GameRunnerView::benchmarkFinished,
          this, &MainWindow::onLevelQuit);
  connect(m_mainMenuView, &Game::Core::MainMenuView::windowClosed, this,
          &MainWindow::onWindowClosed);
  connect(m_mainMenuView, &Game::Core::MainMenuView::newGameSelected, this,
          &MainWindow::newGame);
  connect(m_mainMenuView, &Game::Core::MainMenuView::levelSelectorSelected,
          this, &MainWindow::levelSelector);
  connect(m_mainMenuView, &Game::Core::MainMenuView::benchmarkSelected, this,
          &MainWindow::onBenchmarkSelected);
  connect(m_levelSelectorView, &Game::Core::LevelSelectorView::levelStarted,
          this, &MainWindow::onLevelStarted);
  connect(m_levelSelectorView,
          &Game::Core::LevelSelectorView::backToMainMenuSelected, this,
          &MainWindow::onBackToMainMenuSelected);

  connect(m_pauseMenuView, &Game::Core::PauseMenuView::resumeGameSelected, this,
          &MainWindow::resumeGame);
  connect(m_pauseMenuView, &Game::Core::PauseMenuView::quitLevelSelected, this,
          &MainWindow::onLevelQuit);
  connect(m_pauseMenuView, &Game::Core::PauseMenuView::windowClosed, this,
          &MainWindow::onWindowClosed);
  connect(m_benchmarkPromptView,
          &Game::Core::BenchmarkPromptView::startBenchmarkSelected, this,
          &MainWindow::startBenchmark);
  connect(m_benchmarkPromptView,
          &Game::Core::BenchmarkPromptView::backToMainMenuSelected, this,
          &MainWindow::onBackToMainMenuSelected);

  m_stackedWidget->setCurrentWidget(m_mainMenuView);

  m_levelLoader.setGameCtx(gameCtx);
  m_levelLoader.initialize();
  m_levels = m_levelLoader.loadLevels();
  m_benchmarkLevel = m_levelLoader.loadBenchmarkLevel();
  m_levelSelectorView->setLevelData(m_levels);
  resize(screenWidth, screenHeight);

#ifdef NDEBUG
  showFullScreen();
#else
  show();
#endif
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::newGame() {
  QApplication::setOverrideCursor(Qt::BlankCursor);
  m_stackedWidget->setCurrentWidget(m_gameRunnerView);
  m_gameRunnerView->startLevel(m_levels[1]);
}

void MainWindow::resumeGame() {
  QApplication::setOverrideCursor(Qt::BlankCursor);
  m_stackedWidget->setCurrentWidget(m_gameRunnerView);
  m_gameRunnerView->resumeGame();
}

void MainWindow::levelSelector() {
  m_stackedWidget->setCurrentWidget(m_levelSelectorView);
}

void MainWindow::onGamePaused() {
  QApplication::restoreOverrideCursor();
  m_stackedWidget->setCurrentWidget(m_pauseMenuView);
}

void MainWindow::onLevelQuit() {
  m_gameRunnerView->quitLevel();
  QApplication::restoreOverrideCursor();
  m_stackedWidget->setCurrentWidget(m_mainMenuView);
}

void MainWindow::adjustGameRunnerSize() {
  if (m_gameRunnerView) {
    QRectF newRect(0, 0, width(), height());
    // m_gameRunnerView->scene()->setSceneRect(newRect);
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

void MainWindow::onLevelStarted(Game::Levels::Level level) {
  QApplication::setOverrideCursor(Qt::BlankCursor);
  m_stackedWidget->setCurrentWidget(m_gameRunnerView);
  m_gameRunnerView->startLevel(level);
}

void MainWindow::onBackToMainMenuSelected() {
  m_stackedWidget->setCurrentWidget(m_mainMenuView);
}

void MainWindow::onBenchmarkSelected() {
  m_stackedWidget->setCurrentWidget(m_benchmarkPromptView);
}

void MainWindow::startBenchmark() {
  QApplication::setOverrideCursor(Qt::BlankCursor);
  m_stackedWidget->setCurrentWidget(m_gameRunnerView);
  m_gameRunnerView->startLevel(m_benchmarkLevel, true);
}
