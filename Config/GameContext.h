#pragma once

#include <QRect>
#include <QRectF>

namespace Config {

struct GameContext {
  QRect screenGeometry;
  QRectF movementBounds;

  GameContext(const QRect &geometry) : screenGeometry(geometry) {
    // Calculate movement bounds similarly to your previous method
    qreal minX = 0;
    qreal maxX = screenGeometry.width() * 0.98;
    qreal minY = 0;
    qreal maxY = screenGeometry.height() * 0.865;
    movementBounds = QRectF(minX, minY, maxX - minX, maxY - minY);
  }
};

} // namespace Config
