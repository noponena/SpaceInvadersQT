#include "PlayerShip.h"
#include "GameObjects/Collectables/Collectable.h"
#include "GameObjects/Projectiles/Projectile.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const float speed, const Position &position)
    : Ship(0, speed, position) {
  m_magnetism = {true, true, 100.0f, 100.0f};
  m_pixmapData.pixmapResourcePath = ":/Images/player_ship.png";
  m_pixmapData.pixmapScale = QPointF(50.0, 50.0);
}

void PlayerShip::update(const UpdateContext &context) {
  Ship::update(context);
  regenerateEnergy(context.deltaTimeInSeconds);
  emit playerEnergyUpdated(m_currentEnergy);
}

void PlayerShip::initializeObjectType() {
  Ship::initializeObjectType();
  m_objectTypes.insert(ObjectType::PLAYER_SHIP);
}

void PlayerShip::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::PLAYER_DESTROYED});
}

std::unique_ptr<GameObject> PlayerShip::clone() const {
  std::unique_ptr<PlayerShip> playerShip =
      std::make_unique<PlayerShip>(m_speed, m_position);
  playerShip->m_pixmapData = m_pixmapData;
  playerShip->m_destructionSoundInfo = m_destructionSoundInfo;
  playerShip->m_objectTypes = m_objectTypes;
  playerShip->m_magnetism = m_magnetism;
  playerShip->m_maxHealth = m_maxHealth;
  playerShip->m_maxEnergy = m_maxEnergy;
  playerShip->m_currentEnergy = m_currentEnergy;
  playerShip->m_currentHealth = m_currentHealth;
  playerShip->m_currentSpeedX = m_currentSpeedX;
  playerShip->m_currentSpeedY = m_currentSpeedY;
  playerShip->m_acceleration = m_acceleration;
  return playerShip;
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

float PlayerShip::maxEnergy() const { return m_maxEnergy; }

void PlayerShip::setMaxEnergy(float newMaxEnergy) {
  m_maxEnergy = newMaxEnergy;
  emit playerMaxEnergySet(m_maxEnergy);
}

float PlayerShip::maxHealth() const { return m_maxHealth; }

void PlayerShip::setMaxHealth(float newMaxHealth) {
  m_maxHealth = newMaxHealth;
  emit playerMaxHealthSet(newMaxHealth);
  emit playerHealthUpdated(newMaxHealth);
}

void PlayerShip::setSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon,
                                    unsigned weaponIndex) {
  Ship::setSecondaryWeapon(std::move(newWeapon), weaponIndex);
  emit playerSecondaryWeaponsChanged(m_secondaryWeapons);
}

bool PlayerShip::fireSecondaryWeapon(unsigned int weaponIndex) {
  bool success = Ship::fireSecondaryWeapon(weaponIndex);
  if (success) {
    unsigned cooldownMs = m_secondaryWeapons[weaponIndex]->cooldownMs();
    emit playerSecondaryWeaponFired(weaponIndex, cooldownMs);
    emit playerEnergyUpdated(m_currentEnergy);
  }
  return success;
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
  emit playerHealthUpdated(m_currentHealth);
  if (!isDead())
    playOnHitAnimation();
}

void PlayerShip::collideWithCollectable(
    Collectables::Collectable &collectable) {

  std::unordered_set<ObjectType> types = collectable.objectTypes();
  if (types.find(ObjectType::STELLAR_COIN) != types.end())
    emit stellarTokenCollected();
  else if (types.find(ObjectType::HEALTH) != types.end()) {
    heal(1);
    emit playerHealthUpdated(m_currentHealth);
  }
}

void PlayerShip::disableMovement() {
  m_currentSpeedX = 0;
  m_currentSpeedY = 0;
  m_acceleration = 0;
}
} // namespace Ships

} // namespace GameObjects
