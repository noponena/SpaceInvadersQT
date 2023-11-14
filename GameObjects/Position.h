#ifndef POSITION_H
#define POSITION_H

#include <QPointF>
#include <QRectF>

namespace GameObjects {
class Position {

public:
  Position(float x, float y, float minX = -1, float maxX = -1, float minY = -1,
           float maxY = -1)
      : pos(x, y), anchorPos(x, y),
        bounds(minX, minY, maxX - minX, maxY - minY) {}

  QPointF pos, anchorPos;
  QRectF bounds;

  bool isBeyondScreenTopLimit(float offset = 0) const {
    return pos.y() + offset < bounds.top();
  }

  bool isBeyondScreenBottomLimit(float offset = 0) const {
    return pos.y() - offset > bounds.bottom();
  }

  bool isBeyondScreenLeftLimit(float offset = 0) const {
    return pos.x() + offset < bounds.left();
  }

  bool isBeyondScreenRightLimit(float offset = 0) const {
    return pos.x() - offset > bounds.right();
  }

  bool isBeyondAnyLimit(float offset = 0) const {
    return isBeyondScreenTopLimit(offset) ||
           isBeyondScreenBottomLimit(offset) ||
           isBeyondScreenLeftLimit(offset) || isBeyondScreenRightLimit(offset);
  }

  float x() const { return pos.x(); }
  float y() const { return pos.y(); }

  void setX(float x) { pos.setX(x); }
  void setY(float y) { pos.setY(y); }

  const QRectF &getBounds() const { return bounds; }

  void setBounds(const QRectF &newBounds) { bounds = newBounds; }

  void goToTopLimit() { pos.setY(bounds.top()); }
  void goToBottomLimit() { pos.setY(bounds.bottom()); }
  void goToLeftLimit() { pos.setX(bounds.left()); }
  void goToRightLimit() { pos.setX(bounds.right()); }
  void setPos(QPointF newPos) { pos = newPos; };
};

} // namespace GameObjects

#endif // POSITION_H
