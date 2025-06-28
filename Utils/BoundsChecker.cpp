#include "BoundsChecker.h"

namespace Utils {
bool BoundsChecker::isBeyondScreenTop(const QVector2D &pos,
                                      const QRectF &bounds, float offset) {
  return pos.y() + offset < bounds.top();
}

bool BoundsChecker::isBeyondScreenBottom(const QVector2D &pos,
                                         const QRectF &bounds, float offset) {
  return pos.y() - offset > bounds.bottom();
}

bool BoundsChecker::isBeyondScreenLeft(const QVector2D &pos,
                                       const QRectF &bounds, float offset) {
  return pos.x() + offset < bounds.left();
}

bool BoundsChecker::isBeyondScreenRight(const QVector2D &pos,
                                        const QRectF &bounds, float offset) {
  return pos.x() - offset > bounds.right();
}

bool BoundsChecker::isBeyondAnyLimit(const QVector2D &pos, const QRectF &bounds,
                                     float offset) {
  return isBeyondScreenTop(pos, bounds, offset) ||
         isBeyondScreenBottom(pos, bounds, offset) ||
         isBeyondScreenLeft(pos, bounds, offset) ||
         isBeyondScreenRight(pos, bounds, offset);
}

bool BoundsChecker::isBeyondLimits(const QVector2D &pos, const QRectF &bounds,
                                   float leftOffset, float rightOffset,
                                   float topOffset, float bottomOffset) {
  return isBeyondScreenTop(pos, bounds, topOffset) ||
         isBeyondScreenBottom(pos, bounds, bottomOffset) ||
         isBeyondScreenLeft(pos, bounds, leftOffset) ||
         isBeyondScreenRight(pos, bounds, rightOffset);
}
} // namespace Utils
