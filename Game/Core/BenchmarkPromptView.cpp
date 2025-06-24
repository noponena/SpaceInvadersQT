#include "BenchmarkPromptView.h"

namespace Game {
namespace Core {

BenchmarkPromptView::BenchmarkPromptView(QRect screenGeometry, QWidget *parent)
    : MenuView(screenGeometry, parent) {
  QColor color = QColor(255, 255, 255);

  m_promptText =
      new QGraphicsTextItem("Are you sure you want to run benchmark?");
  m_promptText->setDefaultTextColor(color);
  QFont font;
  font.setPointSize(32);
  m_promptText->setFont(font);
  QRectF textRect = m_promptText->boundingRect();
  m_promptText->setPos((m_screenGeometry.width() - textRect.width()) / 2, 150);
  scene()->addItem(m_promptText);

  createMenuItem("Yes", MenuAction::START_BENCHMARK, color, 100,
                 QPointF(100, 300));
  createMenuItem("No", MenuAction::BACK_TO_MAIN_MENU, color, 100,
                 QPointF(100, 450));
}

} // namespace Core
} // namespace Game
