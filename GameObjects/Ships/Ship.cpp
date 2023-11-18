#include "Ship.h"
#include "Weapons/Weapon.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {

Ship::Ship(const int maxHp, const float speed, const Position &position)
    : GameObject(position), m_immortal(false), m_currentHp(maxHp),
      m_maxHp(maxHp), m_speed(speed) {}

Ship::~Ship() {}

void Ship::shoot() {
  for (const auto &primaryWeapon : m_primaryWeapons) {
    primaryWeapon->shoot();
  }
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

bool Ship::isDead() { return m_currentHp <= 0; }

void Ship::updateFireRate(int amount) {
  for (const auto &primaryWeapon : m_primaryWeapons) {
    primaryWeapon->updateWeaponCooldown(amount);
  }
}

void Ship::addWeapon(std::unique_ptr<Weapons::Weapon> newWeapon) {
  newWeapon->setOwner(this);
  connect(newWeapon.get(), &Weapons::Weapon::projectileShot, this,
          &Ship::onProjectileShot);
  m_primaryWeapons.push_back(std::move(newWeapon));
}

void Ship::clearWeapons() { m_primaryWeapons.clear(); }

void Ship::initializeDestructionAnimation() {
  int columns = 4;
  int rows = 4;
  int targetWidth = 200;
  int targetHeight = 200;

  m_sharedPixmap = Ship::loadSharedSpriteSheet(":/Images/explosion.png")
                       .scaled(targetWidth, targetHeight, Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation);

  std::vector<QPoint> frameOffsets;
  // Calculate and store offsets for each frame
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < columns; ++col) {
      int x = col * targetWidth / 4;
      int y = row * targetHeight / 4;
      frameOffsets.emplace_back(x, y);
    }
  }

  m_destructionAnimation.setSpritesheet(m_sharedPixmap);
  m_destructionAnimation.setFrameOffsets(frameOffsets);
  m_destructionAnimation.setFrameSize(QSize(50, 50));
}

void Ship::initializeDestructionEffects() {
  m_destructionEffect.spawnParticles(250);
}

void Ship::onProjectileShot(
    std::shared_ptr<Projectiles::Projectile> projectile) {
  emit objectCreated(std::move(projectile));
}

void Ship::setImmortal(bool newImmortal) { m_immortal = newImmortal; }

void Ship::setAutoShoot(bool newAutoShoot) { m_autoShoot = newAutoShoot; }

const Magnetism &Ship::magnetism() const { return m_magnetism; }

int Ship::currentHp() const { return m_currentHp; }

bool Ship::shouldBeDeleted() {
  return GameObject::shouldBeDeleted() ||
         (m_destructionAnimation.animationFinished() &&
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

void Ship::update(const UpdateContext &context) {
  GameObject::update(context);
  if (m_autoShoot)
    shoot();
}

} // namespace Ships
} // namespace GameObjects
