#ifndef ANIMATEDITEM_H
#define ANIMATEDITEM_H

#include <QElapsedTimer>
#include <QGraphicsItem>
#include <QPainter>

namespace Graphics {
namespace Animations {

class AnimatedItem : public QGraphicsItem {
public:
  AnimatedItem(QGraphicsItem *parent = nullptr, int animationCycles = 1,
               int delayBetweenFramesMs = 50)
      : QGraphicsItem(parent), m_animationStarted(false),
        m_animationFinished(false), m_accumulatedTimeMs(0),
        m_delayBetweenFramesMs(delayBetweenFramesMs), m_frameIndex(0),
        m_frameCount(0), m_cycleIndex(0), m_cycleCount(animationCycles) {
    hide();
  }

  void start() {
    m_animationStarted = true;
    m_animationFinished = false;
    m_accumulatedTimeMs = 0;
    m_frameIndex = 0;
    m_cycleIndex = 0;
    m_elapsedTimer.start();
  }

  void stop() {
    m_animationStarted = false;
    hide();
  }

  void showNextFrame() {
    if (m_animationStarted && !m_animationFinished) {
      m_accumulatedTimeMs += m_elapsedTimer.restart();
      if (m_accumulatedTimeMs >= m_delayBetweenFramesMs) {
        setSourceRect(QRect(m_frameOffsets[m_frameIndex++], m_frameSize));
        checkCycleFinished();
        checkAnimationFinished();
        m_accumulatedTimeMs = 0;
        show();
      }
    } else {
      hide();
    }
  }

  operator bool() const { return !m_spritesheet.isNull(); }

  QRectF boundingRect() const override { return m_sourceRect; }

  bool animationFinished() const {
    if (m_spritesheet.isNull())
      return true;
    return m_animationFinished;
  }

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
  bool m_animationStarted;
  bool m_animationFinished;
  int m_accumulatedTimeMs;
  int m_delayBetweenFramesMs;
  int m_frameIndex;
  int m_frameCount;
  int m_cycleIndex;
  int m_cycleCount;
  QSize m_frameSize;
  QPixmap m_spritesheet;
  std::vector<QPoint> m_frameOffsets;
  QRect m_sourceRect;
  QElapsedTimer m_elapsedTimer;

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
    m_animationFinished = m_cycleIndex >= m_cycleCount;
  }
};

} // namespace Animations
} // namespace Graphics
#endif // ANIMATEDITEM_H
