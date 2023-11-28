#include "PlayerShip.h"
#include "GameObjects/Collectables/Collectable.h"
#include "GameObjects/Projectiles/Projectile.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const int maxHp, const float speed,
                       const Position &position)
    : ShipWithHealthBar(maxHp, speed, position) {
  m_magnetism = {true, true, 100.0f, 100.0f};
  m_pixmapData.pixmapResourcePath = ":/Images/player_ship.png";
  m_pixmapData.pixmapScale = QPointF(50.0, 50.0);
}

void PlayerShip::initializeObjectType() {
  Ship::initializeObjectType();
  m_objectTypes.insert(ObjectType::PLAYER_SHIP);
}

void PlayerShip::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::PLAYER_DESTROYED});
}

void PlayerShip::moveHorizontal(float deltaTimeInSeconds) {
  moveX(m_currentSpeedX * deltaTimeInSeconds);
}

void PlayerShip::moveVertical(float deltaTimeInSeconds) {
  moveY(m_currentSpeedY * deltaTimeInSeconds);
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
  clampToXBounds();
}

void PlayerShip::moveY(float amount) {
  float current = m_position.y();
  m_position.setY(current + amount);
  clampToYBounds();
}

void PlayerShip::collideWith(GameObject &other) {
  other.collideWithPlayerShip(*this);
}

void PlayerShip::collideWithProjectile(Projectiles::Projectile &projectile) {

  takeDamage(projectile.getDamage());
  if (!isDead())
    playOnHitAnimation();
}

void PlayerShip::collideWithCollectable(
    Collectables::Collectable &collectable) {

  std::unordered_set<ObjectType> types = collectable.objectTypes();
  if (types.find(ObjectType::STELLAR_COIN) != types.end())
    emit stellarTokenCollected();
  else if (types.find(ObjectType::HEALTH) != types.end())
    heal(1);
}

void PlayerShip::disableMovement() {
  m_currentSpeedX = 0;
  m_currentSpeedY = 0;
  m_acceleration = 0;
}
} // namespace Ships

} // namespace GameObjects
