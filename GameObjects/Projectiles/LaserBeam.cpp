#include "LaserBeam.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
LaserBeam::LaserBeam(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed), m_color(color) {}

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
