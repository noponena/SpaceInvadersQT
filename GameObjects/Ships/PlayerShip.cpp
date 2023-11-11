#include "PlayerShip.h"
#include "GameObjects/Projectiles/Projectile.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const int maxHp, float speed, const Position &position)
    : Ship(maxHp, speed, position)
{
    m_objectType = ObjectType::PLAYER_SHIP;
    m_pixmapResourcePath = ":/Images/player_ship.png";
    m_pixmapScale = QPointF(50.0, 75.0);
    m_destructionSoundInfo = Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::PLAYER_DESTROYED});
}

void PlayerShip::collideWith(GameObject &other) {
  other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectiles::Projectile &projectile) {
  if (projectile.hostile())
  {
      this->takeDamage(projectile.getDamage());
      if (!this->isDead())
          this->playOnHitAnimation();
  }
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

bool PlayerShip::shouldBeDeleted()
{
    return false;
}

} // namespace Ships

} // namespace GameObjects
