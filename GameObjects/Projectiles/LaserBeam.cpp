#include "LaserBeam.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
LaserBeam::LaserBeam(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed), m_color(color) {}

QRectF LaserBeam::boundingRect() const { return m_nonTransparentBoundingRect; }

QRectF LaserBeam::getNonTransparentBoundingRect() {
  int minX = getPixmap().width();
  int minY = getPixmap().height();
  int maxX = 0;
  int maxY = 0;
  bool foundNonTransparent = false;

  QImage image = getPixmap().toImage();

  for (int y = 0; y < image.height(); ++y) {
    for (int x = 0; x < image.width(); ++x) {
      QColor pixelColor = image.pixelColor(x, y);
      if (pixelColor.alpha() > 0) { // or another threshold if needed
        foundNonTransparent = true;
        if (x < minX)
          minX = x;
        if (x > maxX)
          maxX = x;
        if (y < minY)
          minY = y;
        if (y > maxY)
          maxY = y;
      }
    }
  }

  if (foundNonTransparent) {
    return QRectF(minX, minY, maxX - minX + 1, maxY - minY + 1);
  } else {
    return QRectF();
  }
}

QPointF LaserBeam::getPixmapScaledSize() const { return QPointF(29.055, 30.0); }

QString LaserBeam::getPixmapResourcePath() const {
  return QString(":/Images/player_laser_projectile.png");
}

QPixmap LaserBeam::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

} // namespace Projectiles

} // namespace GameObjects
