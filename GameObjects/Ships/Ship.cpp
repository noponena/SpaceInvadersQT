#include "Ship.h"
#include "Graphics/PixmapLibrary.h"
#include "Weapons/Weapon.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {

Ship::Ship(const int maxHp, const float speed, const Position &position)
    : AttractableGameObject(position), m_immortal(false), m_currentHp(maxHp),
      m_maxHp(maxHp), m_pixelWidth(50), m_pixelHeight(50),
      m_destructionParticleCount(200), m_activeSecondaryWeaponIndex(0),
      m_speed(speed) {}

Ship::~Ship() {}

void Ship::firePrimaryWeapons() {
  for (const auto &primaryWeapon : m_primaryWeapons) {
    primaryWeapon->fire();
  }
}

void Ship::fireActiveSecondaryWeapon() {
  if (!m_secondaryWeapons.empty())
    m_secondaryWeapons[m_activeSecondaryWeaponIndex]->fire();
}

void Ship::takeDamage(int amount) {
  if (!m_immortal)
    m_currentHp -= amount;
}

void Ship::heal(int amount) {
  if (!isDead()) {
    m_currentHp += amount;
    if (m_currentHp > m_maxHp) {
      m_currentHp = m_maxHp;
    }
  }
}

void Ship::kill() { m_currentHp = 0; }

void Ship::restoreHealth() { m_currentHp = m_maxHp; }

bool Ship::isDead() { return m_currentHp <= 0; }

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

void Ship::addSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon) {
  newWeapon->setOwner(this);
  connect(newWeapon.get(), &Weapons::Weapon::projectileFired, this,
          &Ship::onProjectileFired);
  m_secondaryWeapons.push_back(std::move(newWeapon));
}

void Ship::clearWeapons() {
  m_primaryWeapons.clear();
  m_secondaryWeapons.clear();
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

void Ship::setAutoShoot(bool newAutoShoot) { m_autoShoot = newAutoShoot; }

void Ship::setDestructionParticleCount(int newDestructionParticleCount) {
  m_destructionParticleCount = newDestructionParticleCount;
}

int Ship::currentHp() const { return m_currentHp; }

bool Ship::shouldBeDeleted() {
  if (m_position.isBeyondScreenBottomLimit())
    emit bottomEdgeReached();
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
  if (m_autoShoot)
    firePrimaryWeapons();
}

} // namespace Ships
} // namespace GameObjects
