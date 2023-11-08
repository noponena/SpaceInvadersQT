#include "PlayerShip.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const int maxHp, float speed, const Position &position)
    : Ship(maxHp, speed, position)
{
    m_pixmapResourcePath = ":/Images/player_ship.png";
    m_pixmapScale = QPointF(50.0, 75.0);
}

void PlayerShip::collideWith(GameObject &other) {
  other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectiles::Projectile &projectile) {
    Q_UNUSED(projectile)
}

void PlayerShip::collideWithStellarToken(Collectables::Stellar &stellarToken)
{
    Q_UNUSED(stellarToken);
    emit stellarTokenCollected();
}

QPixmap PlayerShip::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}
} // namespace Ships

} // namespace GameObjects
