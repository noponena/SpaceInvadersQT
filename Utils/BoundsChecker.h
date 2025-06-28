#pragma once

#include <QRectF>
#include <QVector2D>

namespace Utils {
class BoundsChecker {
public:
  // Returns true if pos is above the top edge (with optional offset)
  static bool isBeyondScreenTop(const QVector2D &pos, const QRectF &bounds,
                                float offset = 0.0f);

  // Returns true if pos is below the bottom edge (with optional offset)
  static bool isBeyondScreenBottom(const QVector2D &pos, const QRectF &bounds,
                                   float offset = 0.0f);

  // Returns true if pos is left of the left edge (with optional offset)
  static bool isBeyondScreenLeft(const QVector2D &pos, const QRectF &bounds,
                                 float offset = 0.0f);

  // Returns true if pos is right of the right edge (with optional offset)
  static bool isBeyondScreenRight(const QVector2D &pos, const QRectF &bounds,
                                  float offset = 0.0f);

  // Returns true if pos is out of any edge (with optional offset)
  static bool isBeyondAnyLimit(const QVector2D &pos, const QRectF &bounds,
                               float offset = 0.0f);

  // Returns true if pos is out of specified edges (with per-edge offsets)
  static bool isBeyondLimits(const QVector2D &pos, const QRectF &bounds,
                             float leftOffset, float rightOffset,
                             float topOffset, float bottomOffset);
};
} // namespace Utils
