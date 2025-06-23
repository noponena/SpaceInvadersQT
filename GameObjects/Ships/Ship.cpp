#include "Ship.h"
#include "Graphics/PixmapLibrary.h"
#include "Weapons/Weapon.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {

Ship::Ship(const std::uint32_t maxHp, const float speed,
           const Position &position)
    : AttractableGameObject(position), m_immortal(false), m_pixelWidth(50),
      m_pixelHeight(50), m_destructionParticleCount(200),
      m_currentHealth(maxHp), m_maxHealth(maxHp), m_speed(speed),
      m_energyRegenerationRate(0) {}

Ship::~Ship() {}

void Ship::firePrimaryWeapons() {
  for (const auto &primaryWeapon : m_primaryWeapons) {
    primaryWeapon->fire();
  }
}

bool Ship::fireSecondaryWeapon(std::uint32_t weaponIndex) {
  if (weaponIndex < 4) {
    Weapons::Weapon *weapon = m_secondaryWeapons[weaponIndex].get();
    std::uint32_t weaponEnergyConsuption = weapon->energyConsuption();
    if (m_currentEnergy >= weaponEnergyConsuption) {
      if (!weapon->fire())
        return false;
      m_currentEnergy -= weaponEnergyConsuption;
      return true;
    }
  }
  return false;
}

void Ship::takeDamage(std::uint32_t amount) {
  if (!m_immortal) {
    if (amount > m_currentHealth)
      m_currentHealth = 0;
    else
      m_currentHealth -= amount;
  }
}

void Ship::heal(std::uint32_t amount) {
  if (!isDead()) {
    m_currentHealth += amount;
    if (m_currentHealth > m_maxHealth) {
      m_currentHealth = m_maxHealth;
    }
  }
}

void Ship::kill() { m_currentHealth = 0; }

void Ship::restoreHealth() { m_currentHealth = m_maxHealth; }

bool Ship::isDead() { return m_currentHealth <= 0; }

void Ship::updateFireRate(int amount) {
  for (const auto &primaryWeapon : m_primaryWeapons) {
    primaryWeapon->updateWeaponCooldown(amount);
  }
}

void Ship::addPrimaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon) {
  newWeapon->setOwner(this);
  connect(newWeapon.get(), &Weapons::Weapon::projectileFired, this,
          &Ship::onProjectileFired);
  m_primaryWeapons.push_back(std::move(newWeapon));
}

void Ship::setSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon,
                              std::uint32_t weaponIndex) {
  newWeapon->setOwner(this);
  connect(newWeapon.get(), &Weapons::Weapon::projectileFired, this,
          &Ship::onProjectileFired);
  m_secondaryWeapons[weaponIndex] = std::move(newWeapon);
}

void Ship::clearWeapons() {
  for (auto &weapon : m_primaryWeapons) {
    disconnect(weapon.get(), &Weapons::Weapon::projectileFired, this,
               &Ship::onProjectileFired);
  }
  m_primaryWeapons.clear();
  for (std::uint32_t i = 0; i < 4; i++) {
    if (m_secondaryWeapons[i]) {
      disconnect(m_secondaryWeapons[i].get(), &Weapons::Weapon::projectileFired,
                 this, &Ship::onProjectileFired);
      m_secondaryWeapons[i] = nullptr;
    }
  }
}

void Ship::initializeDestructionAnimation() {
  int columns = 4;
  int rows = 4;
  int targetWidth = 200;
  int targetHeight = 200;

  QPixmap pixmap = Graphics::PixmapLibrary::getPixmap(
      ":/Images/explosion.png", targetWidth, targetHeight);

  std::vector<QPoint> frameOffsets;
  // Calculate and store offsets for each frame
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < columns; ++col) {
      int x = col * targetWidth / 4;
      int y = row * targetHeight / 4;
      frameOffsets.emplace_back(x, y);
    }
  }

  m_destructionAnimation.setSpritesheet(pixmap);
  m_destructionAnimation.setFrameOffsets(frameOffsets);
  m_destructionAnimation.setFrameSize(QSize(m_pixelWidth, m_pixelHeight));
}

void Ship::initializeDestructionEffects() {
  m_destructionEffect.spawnParticles(m_destructionParticleCount);
}

void Ship::onProjectileFired(
    std::shared_ptr<Projectiles::Projectile> projectile) {
  emit objectCreated(std::move(projectile));
}

void Ship::setImmortal(bool newImmortal) { m_immortal = newImmortal; }

void Ship::setAutoShoot(bool newAutoShoot) { m_autoFire = newAutoShoot; }

void Ship::setDestructionParticleCount(int newDestructionParticleCount) {
  m_destructionParticleCount = newDestructionParticleCount;
}

void Ship::fullyRestoreEnergy() { m_currentEnergy = m_maxEnergy; }

void Ship::fullyRestoreHealth() { m_currentHealth = m_maxHealth; }

std::uint32_t Ship::energyRegenerationRate() const {
  return m_energyRegenerationRate;
}

void Ship::setEnergyRegenerationRate(std::uint32_t newEnergyRegenerationRate) {
  m_energyRegenerationRate = newEnergyRegenerationRate;
}

void Ship::setMaxHealth(float newMaxHealth) { m_maxHealth = newMaxHealth; }

void Ship::setSpeed(float newSpeed) { m_speed = newSpeed; }

void Ship::setMaxEnergy(float newMaxEnergy) { m_maxEnergy = newMaxEnergy; }

int Ship::currentHp() const { return m_currentHealth; }

bool Ship::shouldBeDeleted() {
  return GameObject::shouldBeDeleted() ||
         (isDead() && m_destructionAnimation.animationFinished() &&
          m_destructionEffect.effectFinished());
}

void Ship::playOnHitAnimation() {
  if (m_onHitAnimationInProgress)
    return;

  QPixmap onHitPixmap = getOnHitPixmap();
  m_onHitAnimationInProgress = true;
  m_graphicsItem->setPixmap(onHitPixmap);
  m_onHitTimerId = startTimer(100);
}

void Ship::regenerateEnergy(float deltaTimeInSeconds) {
  m_currentEnergy += m_energyRegenerationRate * deltaTimeInSeconds;
  if (m_currentEnergy > m_maxEnergy)
    m_currentEnergy = m_maxEnergy;
  else if (m_currentEnergy < 0)
    m_currentEnergy = 0;
}

void Ship::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_onHitTimerId) {
    killTimer(m_onHitTimerId);
    m_graphicsItem->setPixmap(getPixmap());
    m_onHitAnimationInProgress = false;
    m_onHitTimerId = -1;
  }
}

void Ship::initializeObjectType() { m_objectTypes.insert(ObjectType::SHIP); }

void Ship::update(const UpdateContext &context) {
  AttractableGameObject::update(context);
  if (m_autoFire)
    firePrimaryWeapons();
}

} // namespace Ships
} // namespace GameObjects
