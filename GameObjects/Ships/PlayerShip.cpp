#include "PlayerShip.h"
#include "GameObjects/Projectiles/Projectile.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const int maxHp, float speed, const Position &position)
    : Ship(maxHp, position), m_speed(speed) {
  m_objectType = ObjectType::PLAYER_SHIP;
  m_pixmapResourcePath = ":/Images/player_ship.png";
  m_pixmapScale = QPointF(50.0, 75.0);
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::PLAYER_DESTROYED});
  connect(this, &Ship::destructionCompleted, this,
          &PlayerShip::playerShipDestroyed);
}

void PlayerShip::moveHorizontal(float deltaTimeInSeconds) {
  this->moveX(m_currentSpeedX * deltaTimeInSeconds);
}

void PlayerShip::moveVertical(float deltaTimeInSeconds) {
  this->moveY(m_currentSpeedY * deltaTimeInSeconds);
}

void PlayerShip::accelerateLeft(float deltaTimeInSeconds) {
  m_currentSpeedX -= m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedX < -m_speed) {
    m_currentSpeedX = -m_speed;
  }
}

void PlayerShip::accelerateRight(float deltaTimeInSeconds) {
  m_currentSpeedX += m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedX > m_speed) {
    m_currentSpeedX = m_speed;
  }
}

void PlayerShip::decelerateX(float deltaTimeInSeconds) {
  if (m_currentSpeedX > 0) {
    m_currentSpeedX -= m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedX < 0) {
      m_currentSpeedX = 0;
    }
  } else if (m_currentSpeedX < 0) {
    m_currentSpeedX += m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedX > 0) {
      m_currentSpeedX = 0;
    }
  }
}

void PlayerShip::accelerateUp(float deltaTimeInSeconds) {
  m_currentSpeedY -= m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedY < -m_speed) {
    m_currentSpeedY = -m_speed;
  }
}

void PlayerShip::accelerateDown(float deltaTimeInSeconds) {
  m_currentSpeedY += m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedY > m_speed) {
    m_currentSpeedY = m_speed;
  }
}

void PlayerShip::decelerateY(float deltaTimeInSeconds) {
  if (m_currentSpeedY > 0) {
    m_currentSpeedY -= m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedY < 0) {
      m_currentSpeedY = 0;
    }
  } else if (m_currentSpeedY < 0) {
    m_currentSpeedY += m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedY > 0) {
      m_currentSpeedY = 0;
    }
  }
}

void PlayerShip::moveX(float amount) {
  float current = m_position.x();
  m_position.setX(current + amount);
  this->clampToXBounds();
}

void PlayerShip::moveY(float amount) {
  float current = m_position.y();
  m_position.setY(current + amount);
  this->clampToYBounds();
}

void PlayerShip::collideWith(GameObject &other) {
  other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectiles::Projectile &projectile) {

  this->takeDamage(projectile.getDamage());
  if (!this->isDead())
    this->playOnHitAnimation();
}

void PlayerShip::collideWithStellarToken(Collectables::Stellar &stellarToken) {
  Q_UNUSED(stellarToken);
  emit stellarTokenCollected();
}

QPixmap PlayerShip::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

void PlayerShip::disableMovement() {
  m_currentSpeedX = 0;
  m_currentSpeedY = 0;
  m_acceleration = 0;
}

void PlayerShip::initialize()
{
  GameObject::initialize();
  m_healthBar = std::make_unique<HealthBar>(m_currentHp, 50, 5, this->m_graphicsItem);
  m_healthBar->setPos(0, 45); // Position it below the ship
}

void PlayerShip::takeDamage(int amount)
{
  Ship::takeDamage(amount);
  m_healthBar->updateHealth(-amount);
}

} // namespace Ships

} // namespace GameObjects
