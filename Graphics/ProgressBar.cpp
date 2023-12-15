#include "ProgressBar.h"
#include <QBrush>

namespace Graphics {

ProgressBar::ProgressBar(float maxProgress, float width, float height,
                         bool showNumericProgression, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_currentProgress(maxProgress),
      m_maxProgress(maxProgress), m_progressPercentage(1.0f), m_maxWidth(width),
      m_height(height), m_currentWidth(width),
      m_showNumbericProgression(showNumericProgression) {}

QRectF ProgressBar::boundingRect() const {
  return QRectF(0, 0, m_maxWidth, m_height);
}

void ProgressBar::updateProgress(float amount) {
  m_currentProgress += amount;
  clampCurrentProgress();
  m_progressPercentage = m_currentProgress / m_maxProgress;
  m_currentWidth = m_maxWidth * m_progressPercentage;
}

void ProgressBar::setProgress(float newProgress) {
  m_currentProgress = newProgress;
  clampCurrentProgress();
  m_progressPercentage = m_currentProgress / m_maxProgress;
  m_currentWidth = m_maxWidth * m_progressPercentage;
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
  painter->save();
  painter->setBrush(QColor(98, 98, 103));
  painter->drawRect(boundingRect());

  painter->setBrush(selectBrush());
  QRectF filledRect(0, 0, m_currentWidth, m_height);
  painter->drawRect(filledRect);

  if (m_showNumbericProgression) {
    QString progressText =
        QString::number(static_cast<int>(m_currentProgress)) + " / " +
        QString::number(m_maxProgress);

    QFontMetrics fm(painter->font());
    int textWidth = fm.horizontalAdvance(progressText);
    int textHeight = fm.height();
    QPointF textPos((m_maxWidth - textWidth) / 2,
                    (m_height - textHeight) / 2 + fm.ascent());

    painter->setPen(Qt::white);
    painter->drawText(textPos, progressText);
  }
  painter->restore();
}

} // namespace Graphics
