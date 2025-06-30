#include "Ship.h"
#include "Graphics/TextureRegistry.h"
#include "Weapons/Weapon.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {

Ship::Ship(const Config::GameContext &ctx)
    : AttractableGameObject(ctx), m_immortal(false), m_pixelWidth(50),
      m_pixelHeight(50), m_destructionParticleCount(200), m_currentHealth(1),
      m_maxHealth(1), m_speed(1), m_energyRegenerationRate(0) {}

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

    qDebug() << "A ship took dmg for " << amount
             << ". HP remaining: " << m_currentHealth;
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
  auto renderDataIt = m_renderDataByState.find(State::OnDestruction);
  if (renderDataIt == m_renderDataByState.end()) {
    qDebug() << "No destruction spritesheet found.";
    return;
  }

  int columns = 4;
  int rows = 4;

  const QString &texPath = renderDataIt->second.imagePath;
  const auto &texInfo =
      Graphics::TextureRegistry::instance().getOrCreateTexture(texPath);
  QSize sheetSize(texInfo.width, texInfo.height);

  Graphics::Animations::AnimationInfo animInfo;
  animInfo.sheetSize = sheetSize;
  animInfo.columns = columns;
  animInfo.rows = rows;
  animInfo.frameDurationMs = 50;
  animInfo.frameUVs.clear();

  qDebug() << "[Ship] Initializing animation..";

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < columns; ++col) {
      float u0 = float(col) / columns;
      float v0 = float(row) / rows;
      float u1 = float(col + 1) / columns;
      float v1 = float(row + 1) / rows;
      QVector2D uvMin = QVector2D(u0, v0);
      QVector2D uvMax = QVector2D(u1, v1);
      animInfo.frameUVs.emplace_back(uvMin, uvMax);
      qDebug() << "[Ship] Added a frame, uvMin =" << uvMin << "uvMax =" << uvMax;
    }
  }

  m_animationInfoByState[State::OnDestruction] = std::move(animInfo);
}

void Ship::initializeDestructionEffects() {
  m_destructionEffect.spawnParticles(m_destructionParticleCount);
}

void Ship::onProjectileFired(
    std::shared_ptr<GameObjects::Projectiles::Projectile> projectile) {
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

void Ship::setMaxEnergy(float newMaxEnergy) { m_maxEnergy = newMaxEnergy; }

int Ship::currentHp() const { return m_currentHealth; }

bool Ship::shouldBeDeleted() {
  return GameObject::shouldBeDeleted() ||
         (isDead() && m_animationPlayer.isFinished());
}

void Ship::playOnHitAnimation() {
  if (m_onHitAnimationInProgress)
    return;

  setState(State::OnHit);
  m_onHitAnimationInProgress = true;
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
    setState(State::Normal);
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
