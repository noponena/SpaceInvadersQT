#include "Ship.h"
#include "Weapons/Weapon.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QTimer>
#include <QTimerEvent>


namespace GameObjects {
namespace Ships {

Ship::Ship(const int maxHp, int speed, const Position &position)
    : GameObject(position, speed), m_maxHp(maxHp), m_speed(speed),
      m_magnetism({true, 100.0f, 100.0f})
{
  m_currentHp = maxHp;
  m_animationTimer = std::make_unique<QTimer>();
}

Ship::~Ship()
{

}

void Ship::shoot() { m_weapon->shoot(); }

void Ship::takeDamage(int amount) { m_currentHp -= amount; }

void Ship::heal(int amount) {
  if (this->isDead()) {
    m_currentHp += amount;
    if (m_currentHp > m_maxHp) {
      m_currentHp = m_maxHp;
    }
  }
}

bool Ship::isDead() { return m_currentHp <= 0; }

void Ship::updateFireRate(int amount) {
  m_weapon->updateWeaponCooldown(amount);
}

void Ship::setWeapon(std::unique_ptr<Weapons::Weapon> newWeapon) {
  m_weapon = std::move(newWeapon);
  m_weapon->setOwner(this);

  QObject::connect(
      m_weapon.get(), &Weapons::Weapon::projectileShot, this,
      [this](GameObjects::Projectiles::Projectile *projectile)
      { emit this->objectCreated(projectile); });
}

void Ship::initializeDestructionAnimation() {
  int frameWidth = 400;  // width of a single frame
  int frameHeight = 400; // height of a single frame
  int columns = 4;       // number of columns of frames in the sprite sheet
  int rows = 4;          // number of rows of frames in the sprite sheet
  int targetWidth = 50;  // The width of your ship
  int targetHeight = 75; // The height of your ship

  QPixmap sharedSpriteSheet = Ship::loadSharedSpriteSheet(":/Images/explosion.png");

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < columns; ++col) {
      int x = col * frameWidth;
      int y = row * frameHeight;
      QPixmap frame = sharedSpriteSheet.copy(x, y, frameWidth, frameHeight);
      QPixmap scaledFrame =
          frame.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
      m_animationFrames.append(scaledFrame);
    }
  }
  connect(this, &Ship::animationCompleted, this, &Ship::onAnimationCompleted);
}

void Ship::onAnimationCompleted() { m_destroyed = true;}

const magnetism &Ship::magnetism() const
{
  return m_magnetism;
}

int Ship::currentHp() const
{
  return m_currentHp;
}

void Ship::playDestructionAnimation() {
  if (m_onHitTimerId != -1) {
    killTimer(m_onHitTimerId);
    m_onHitTimerId = -1;
  }
  m_frameIndex = 0;
  QGraphicsPixmapItem *pixmapItem =
      qgraphicsitem_cast<QGraphicsPixmapItem *>(m_graphicsItem);
  connect(m_animationTimer.get(), &QTimer::timeout, this, [this, pixmapItem]() {
    if (pixmapItem && m_frameIndex < m_animationFrames.size()) {

      pixmapItem->setPixmap(m_animationFrames[m_frameIndex]);
      m_frameIndex++;
    } else {
      m_animationTimer->stop();
      emit animationCompleted();
    }
  });
  m_animationTimer->start(50);
}

void Ship::playDestructionEffects()
{
  QPointF p(m_position.x() + m_halfWidth, m_position.y() + m_halfHeight);
  m_particleSystem->setPosition(p);
  m_graphicsItem->scene()->addItem(m_particleSystem.get());
}

bool Ship::shouldBeDeleted() { return m_destroyed; }

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

void Ship::initializeDestructionEffects()
{
    QRectF rect = m_graphicsItem->boundingRect();
    m_halfWidth = rect.width() / 2;
    m_halfHeight = rect.height() / 2;
    m_particleSystem = std::make_unique<Effects::ParticleSystem>();
    m_particleSystem->spawnParticles(200);
}
} // namespace Ships
} // namespace GameObjects
