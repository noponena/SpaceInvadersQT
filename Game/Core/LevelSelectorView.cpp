#include "LevelSelectorView.h"

namespace Game {
namespace Core {

LevelSelectorView::LevelSelectorView(QRect screenGeometry, QWidget *parent)
    : MenuView(screenGeometry, parent), m_levelAllowedToStart(false) {
  qDebug() << "screenGeometry:" << screenGeometry;
  QColor color = QColor(255, 255, 255);
  createMenuItem("Back", MenuAction::BACK_TO_MAIN_MENU, color, 32,
                 QPointF((m_screenGeometry.width() / 2) * 0.9,
                         m_screenGeometry.height() * 0.8));
  createMenuItem("Start Level", MenuAction::START_LEVEL, color, 32,
                 QPointF((m_screenGeometry.width() / 2) * 1.1,
                         m_screenGeometry.height() * 0.8));
  connect(this, &LevelSelectorView::levelSelected, this,
          &LevelSelectorView::onLevelSelected);
  connect(this, &LevelSelectorView::startLevelSelected, this,
          &LevelSelectorView::onStartLevelSelected);
}

void LevelSelectorView::setLevelData(
    std::map<int, Game::Levels::Level> levels) {
  int yOffset = 0;
  for (const auto &level : levels) {
    QString levelNumber = QString::number(level.second.levelNumber);
    QString levelName = QString::fromStdString(level.second.name);
    QString displayText =
        QString("Level %1: %2").arg(levelNumber).arg(levelName);
    QVariant payload = QVariant::fromValue(level.second);
    createMenuItem(displayText, MenuAction::SELECT_LEVEL, Qt::white, 16,
                   QPointF(m_screenGeometry.width() / 2, yOffset), payload);
    yOffset += 50;
  }
}

void LevelSelectorView::onLevelSelected(Levels::Level level) {
  m_currentLevel = level;
  m_levelAllowedToStart = true;
}

void LevelSelectorView::onStartLevelSelected() {
  if (m_levelAllowedToStart)
    emit levelStarted(m_currentLevel);
}

void LevelSelectorView::onMenuItemClicked() {
  TogglableMenuTextItem *clickedItem =
      qobject_cast<TogglableMenuTextItem *>(sender());
  if (clickedItem && clickedItem->menuAction() == MenuAction::SELECT_LEVEL) {
    if (m_selectedItem) {
      m_selectedItem->setSelected(false);
    }
    m_selectedItem = clickedItem;
    m_selectedItem->setSelected(true);
  }
  MenuView::onMenuItemClicked();
}

void LevelSelectorView::createMenuItem(const QString &text,
                                       MenuAction menuAction, QColor color,
                                       int fontSize, const QPointF &pos,
                                       QVariant payload) {
  TogglableMenuTextItem *item =
      new TogglableMenuTextItem(text, color, menuAction);
  item->setPayload(payload);
  QFont font;
  font.setPointSize(fontSize);
  item->setFont(font);
  item->setPos(pos);

  connect(item, &TogglableMenuTextItem::clicked, this,
          &LevelSelectorView::onMenuItemClicked);
  scene()->addItem(item);
}

} // namespace Core
} // namespace Game
