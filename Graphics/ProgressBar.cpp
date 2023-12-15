#include "ProgressBar.h"
#include <QBrush>

namespace Graphics {

ProgressBar::ProgressBar(float maxProgress, float width, float height,
                         QGraphicsItem *parent)
    : QGraphicsItem(parent), m_currentProgress(maxProgress),
      m_maxProgress(maxProgress), m_progressPercentage(1.0f), m_width(width),
      m_height(height), m_currentWidth(width)

{}

QRectF ProgressBar::boundingRect() const {
  return QRectF(0, 0, m_width, m_height);
}

void ProgressBar::updateProgress(float amount) {
  m_currentProgress += amount;
  clampCurrentProgress();
  m_progressPercentage = m_currentProgress / m_maxProgress;
  m_currentWidth = m_width * m_progressPercentage;
}

void ProgressBar::setProgress(float progress) {
  m_currentProgress = progress;
  clampCurrentProgress();
  m_progressPercentage = m_currentProgress / m_maxProgress;
  m_currentWidth = m_width * m_progressPercentage;
}

void ProgressBar::setMaxProgress(float newMaxProgress) {
  m_maxProgress = newMaxProgress;
}

float ProgressBar::maxProgress() const { return m_maxProgress; }

void ProgressBar::clampCurrentProgress() {
  if (m_currentProgress > m_maxProgress)
    m_currentProgress = m_maxProgress;
  else if (m_currentProgress < 0)
    m_currentProgress = 0;
}

void ProgressBar::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setBrush(QColor(98, 98, 103));
  painter->drawRect(boundingRect());

  painter->setBrush(selectBrush());
  QRectF filledRect(0, 0, m_currentWidth, m_height);
  painter->drawRect(filledRect);
}

} // namespace Graphics
