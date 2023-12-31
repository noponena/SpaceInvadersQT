#include "LevelSelectorView.h"

namespace Game {
namespace Core {

LevelSelectorView::LevelSelectorView(QRect screenGeometry, QWidget *parent)
    : MenuView(screenGeometry, parent) {}

void LevelSelectorView::setLevelData(
    std::unordered_map<int, Game::Levels::Level> levels) {
  int yOffset = 0;
  for (const auto &level : levels) {
    QString levelNumber = QString::number(level.second.levelNumber);
    QString levelName = QString::fromStdString(level.second.name);
    QString displayText =
        QString("Level %1: %2").arg(levelNumber).arg(levelName);
    QVariant payload = QVariant::fromValue(level);
    createMenuItem(displayText, MenuAction::SELECT_LEVEL, Qt::white, 16,
                   QPointF(100, yOffset), payload);
    yOffset += 50;
  }
}

} // namespace Core
} // namespace Game
