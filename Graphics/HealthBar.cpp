#include "HealthBar.h"

namespace Graphics {

HealthBar::HealthBar(float maxHealth, float width, float height,
                     QGraphicsItem *parent)
    : ProgressBar(maxHealth, width, height, parent) {}

QBrush HealthBar::selectBrush() {
  if (m_progressPercentage < 0.3)
    return QBrush(QColor(211, 82, 105));
  if (m_progressPercentage < 0.6)
    return QBrush(QColor(248, 178, 98));

  return QBrush(QColor(23, 184, 144));
}

} // namespace Graphics
