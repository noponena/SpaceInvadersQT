#include "PlayerShip.h"
#include "GameObjects/Collectables/Collectable.h"
#include "GameObjects/Projectiles/Projectile.h"
#include <QPen>

namespace GameObjects {
namespace Ships {
PlayerShip::PlayerShip(const Config::GameContext &ctx) : Ship(ctx) {
  m_magnetism = {true, true, 100.0f, 100.0f};

  RenderData normalData;
  normalData.size = QVector2D(50, 50);
  normalData.imagePath = ":/Images/player_ship.png";
  addRenderData(State::Normal, normalData);

  RenderData onDestructionData;
  onDestructionData.size = QVector2D(50, 50);
  onDestructionData.imagePath = ":/Images/explosion.png";
  addRenderData(State::OnDestruction, onDestructionData);

  m_transform.colliderSize = {30, 30};

  /*
  RenderData onHitData;
  onHitData.size = QVector2D(100, 100);
  onHitData.imagePath = ":/Images/player_ship.png";
  addRenderData(State::OnHit, onHitData);
 */
}

void PlayerShip::update(const UpdateContext &context) {
  Ship::update(context);
  float prevEnergy = m_currentEnergy;
  regenerateEnergy(context.deltaTimeInSeconds);
  if (prevEnergy != m_currentEnergy)
    emit playerEnergyUpdated(m_currentEnergy);
}

bool PlayerShip::shouldBeDeleted() {
  return (isDead() && m_animationPlayer.isFinished());
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
      std::make_unique<PlayerShip>(m_gameContext);

  playerShip->setTransform(m_transform);
  playerShip->setSpeed(m_speed);
  playerShip->m_renderDataByState = m_renderDataByState;
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
  moveRelativeX(m_currentSpeedX * deltaTimeInSeconds);
}

void PlayerShip::moveVertical(float deltaTimeInSeconds) {
  moveRelativeY(m_currentSpeedY * deltaTimeInSeconds);
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
  Ship::setMaxEnergy(newMaxEnergy);
  emit playerMaxEnergySet(m_maxEnergy);
}

float PlayerShip::maxHealth() const { return m_maxHealth; }

void PlayerShip::setMaxHealth(float newMaxHealth) {
  Ship::setMaxHealth(newMaxHealth);
  emit playerMaxHealthSet(newMaxHealth);
  emit playerHealthUpdated(newMaxHealth);
}

void PlayerShip::setSpeed(float newSpeed) { m_speed = newSpeed; }

void PlayerShip::setSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon,
                                    std::uint32_t weaponIndex) {
  Ship::setSecondaryWeapon(std::move(newWeapon), weaponIndex);
  std::array<QString, 4> paths;
  for (int i = 0; i < 4; ++i)
    paths[i] = m_secondaryWeapons[i] ? m_secondaryWeapons[i]->getHudImagePath()
                                     : "";
  emit playerSecondaryWeaponsChanged(paths);
}

bool PlayerShip::fireSecondaryWeapon(std::uint32_t weaponIndex) {
  bool success = Ship::fireSecondaryWeapon(weaponIndex);
  if (success) {
    std::uint32_t cooldownMs = m_secondaryWeapons[weaponIndex]->cooldownMs();
    emit playerSecondaryWeaponFired(weaponIndex, cooldownMs);
    emit playerEnergyUpdated(m_currentEnergy);
  }
  return success;
}

void PlayerShip::restoreHealth() {
  Ship::restoreHealth();
  emit playerHealthUpdated(m_currentHealth);
}

void PlayerShip::restoreEnergy() {
  Ship::restoreEnergy();
  emit playerEnergyUpdated(m_currentEnergy);
}

void PlayerShip::moveRelativeX(float displacement) {
  moveRelative({displacement, 0});
  clampToXBounds();
}

void PlayerShip::moveRelativeY(float displacement) {
  moveRelative({0, displacement});
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
