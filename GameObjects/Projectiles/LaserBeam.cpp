#include "LaserBeam.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
LaserBeam::LaserBeam(Position position, float speed, Qt::GlobalColor color)
    : Projectile(position, speed), m_color(color)
{
    m_spawnSoundInfo = SoundInfo({true, QUrl("qrc:/Sounds/laser.wav"), 1000});
    m_pixmapResourcePath = ":/Images/player_laser_projectile.png";
    m_pixmapScale = QPointF(29.055, 30.0);
}

QPixmap LaserBeam::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
    return pixmap;
}

} // namespace Projectiles
} // namespace GameObjects
