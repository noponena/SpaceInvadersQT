#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QGraphicsItem>
#include <QPainter>

namespace Animations {

class AnimatedItem : public QGraphicsItem {
public:
  AnimatedItem(QGraphicsItem *parent = nullptr, int animationCycles = 1)
      : QGraphicsItem(parent), m_animationFinished(false), m_frameIndex(0),
        m_frameCount(0), m_cycleIndex(0), m_cycles(animationCycles) {}

  void showNextFrame() {
    setSourceRect(QRect(m_frameOffsets[m_frameIndex++], m_frameSize));
    checkCycleFinished();
    checkAnimationFinished();
  }

  QRectF boundingRect() const override { return m_sourceRect; }

  bool animationFinished() const { return m_animationFinished; }

  void setSpritesheet(const QPixmap &newSpritesheet) {
    m_spritesheet = newSpritesheet;
  }

  void setFrameSize(const QSize &newFrameSize) { m_frameSize = newFrameSize; }

  void setFrameOffsets(const std::vector<QPoint> &newFrameOffsets) {
    m_frameOffsets = newFrameOffsets;
    m_frameCount = m_frameOffsets.size();
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *) override {
    painter->drawPixmap(0, 0, m_spritesheet, m_sourceRect.x(), m_sourceRect.y(),
                        m_sourceRect.width(), m_sourceRect.height());
  }

private:
  bool m_animationFinished;
  int m_frameIndex;
  int m_frameCount;
  int m_cycleIndex;
  int m_cycles;
  QSize m_frameSize;
  QPixmap m_spritesheet;
  std::vector<QPoint> m_frameOffsets;
  QRect m_sourceRect;

  void setSourceRect(const QRect &rect) {
    m_sourceRect = rect;
    update();
  }

  void checkCycleFinished() {
    if (m_frameIndex >= m_frameCount) {
      m_frameIndex = 0;
      m_cycleIndex++;
    }
  }

  void checkAnimationFinished() {
    m_animationFinished = m_cycleIndex >= m_cycles;
  }
};

} // namespace Animations

#endif // ANIMATEDITEM_H
