#ifndef GRAPHICS_PROGRESSBAR_H
#define GRAPHICS_PROGRESSBAR_H

#include <QGraphicsItem>
#include <QPainter>

namespace Graphics {

class ProgressBar : public QGraphicsItem {
public:
  ProgressBar(float maxProgress, float width, float height,
              bool showNumericProgression = false,
              QGraphicsItem *parent = nullptr);
  QRectF boundingRect() const override;
  void updateProgress(float amount);
  void setProgress(float newProgress);
  float maxProgress() const;
  void setMaxProgress(float newMaxProgress);

protected:
  float m_currentProgress;
  float m_maxProgress;
  float m_progressPercentage;
  float m_maxWidth;
  float m_height;
  float m_currentWidth;

  virtual QBrush selectBrush() = 0;

private:
  bool m_showNumbericProgression;
  void clampCurrentProgress();

  // QGraphicsItem interface
public:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
};

} // namespace Graphics

#endif // GRAPHICS_PROGRESSBAR_H
