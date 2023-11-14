#include "Ship.h"
#include "Weapons/Weapon.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {

Ship::Ship(const int maxHp, const Position &position)
    : GameObject(position), m_maxHp(maxHp),
      m_magnetism({true, 100.0f, 100.0f}) {
  m_currentHp = maxHp;
}

Ship::~Ship() {}

void Ship::shoot() {
  for (const auto &primaryWeapon : m_primaryWeapons) {
    primaryWeapon->shoot();
  }
}

void Ship::takeDamage(int amount) { m_currentHp -= amount; }

void Ship::heal(int amount) {
  if (!this->isDead()) {
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

  connect(this, &Ship::destructionCompleted, this,
          &Ship::onDestructionCompleted);
}

void Ship::onDestructionCompleted() { m_destructionCompleted = true; }

void Ship::onProjectileShot(
    std::shared_ptr<Projectiles::Projectile> projectile) {
  emit this->objectCreated(std::move(projectile));
}

const magnetism &Ship::magnetism() const { return m_magnetism; }

int Ship::currentHp() const { return m_currentHp; }

void Ship::playDestructionAnimation() {
  if (m_onHitTimerId != -1) {
    killTimer(m_onHitTimerId);
    m_onHitTimerId = -1;
  }

  m_graphicsItem->hide();
  m_destructionAnimation.hide();
  m_graphicsItem->scene()->addItem(&m_destructionAnimation);
  m_destructionAnimation.setPos(m_graphicsItem->pos());
  connect(&m_animationTimer, &QTimer::timeout, this, [this]() {
    if (!m_destructionAnimation.animationFinished()) {
      m_destructionAnimation.show();
      m_destructionAnimation.showNextFrame();
    } else {
      m_animationTimer.stop();
      emit destructionCompleted();
    }
  });
  m_animationTimer.start(50);
}

void Ship::playDestructionEffects() {
  QPointF p(m_position.x() + m_halfWidth, m_position.y() + m_halfHeight);
  m_destructionEffect.setPosition(p);
  m_graphicsItem->scene()->addItem(&m_destructionEffect);
}

bool Ship::shouldBeDeleted() { return m_destructionCompleted; }

void Ship::playOnHitAnimation() {
  if (m_onHitAnimationInProgress)
    return;

  // Load the "on hit" pixmap and set it to the graphics item
  QPixmap onHitPixmap = this->getOnHitPixmap();
  m_onHitAnimationInProgress = true;
  static_cast<QGraphicsPixmapItem *>(m_graphicsItem)->setPixmap(onHitPixmap);
  m_onHitTimerId = startTimer(100);
}

void Ship::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_onHitTimerId) {
    killTimer(m_onHitTimerId);
    static_cast<QGraphicsPixmapItem *>(m_graphicsItem)
        ->setPixmap(this->getPixmap());
    m_onHitAnimationInProgress = false;
    m_onHitTimerId = -1;
  }
}

void Ship::initializeDestructionEffects() {
  QRectF rect = m_graphicsItem->boundingRect();
  m_halfWidth = rect.width() / 2;
  m_halfHeight = rect.height() / 2;
  m_destructionEffect.spawnParticles(500);
}
} // namespace Ships
} // namespace GameObjects
