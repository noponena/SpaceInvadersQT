#include "PlayerShip.h"
#include "Weapons/LaserCannon.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const int maxHp, float speed, const Position &position)
    : Ship(maxHp, speed, position) {
  m_lastShotTime.start();
}

void PlayerShip::collideWith(GameObject &other) {
  other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectiles::Projectile &projectile) {
  Q_UNUSED(projectile)
}

QPointF PlayerShip::getPixmapScaledSize() const { return QPointF(50.0, 75.0); }

QString PlayerShip::getPixmapResourcePath() const {
  return QString(":/Images/player_ship.png");
}

QString PlayerShip::getOnHitPixmapResourcePath() const {
  return QString(":/Images/player_ship.png");
}

QPixmap PlayerShip::getPixmap() const {
  static QPixmap pixmap = GameObject::getOnHitPixmap();
  return pixmap;
}
} // namespace Ships

} // namespace GameObjects
