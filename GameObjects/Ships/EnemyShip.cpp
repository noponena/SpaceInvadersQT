#include "EnemyShip.h"
#include <QColor>
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {
EnemyShip::EnemyShip(const int maxHp, int speed, const Position &position)
    : Ship(maxHp, speed, position)
{
    m_pixmapResourcePath = ":/Images/alien.png";
    m_onHitPixmapResourcePath = ":/Images/alien_on_hit.png";
    m_pixmapScale = QPointF(50.0, 75.0);
    m_destructionSoundInfo = SoundInfo({true, QUrl("qrc:/Sounds/explosion.wav"), 1000});
}

void EnemyShip::initiateDestructionProcedure() {
    GameObject::initiateDestructionProcedure();
    this->spawnCollectables(5);
}

void EnemyShip::spawnCollectables(int amount)
{
    QPointF center = this->getBoundingBox().center();
    GameObjects::Position position(center.x(), center.y());
    position.setBounds(this->getPosition().getBounds());
    for (int i = 0; i < amount; i++) {
        std::shared_ptr<GameObjects::Collectables::Stellar> stellar = std::make_shared<GameObjects::Collectables::Stellar>(position);
        stellar->initialize();
        emit objectCreated(stellar);
    }
}

void EnemyShip::playOnHitAnimation() {
  if (m_onHitAnimationInProgress)
    return;

  m_onHitAnimationInProgress = true;

  // Load the "on hit" pixmap and set it to the graphics item
  QPixmap onHitPixmap = this->getOnHitPixmap();
  static_cast<QGraphicsPixmapItem *>(m_graphicsItem)->setPixmap(onHitPixmap);

  m_onHitTimerId = startTimer(100);
}

void EnemyShip::timerEvent(QTimerEvent *event) {
  if (event->timerId() == m_onHitTimerId) {
    killTimer(m_onHitTimerId);
    static_cast<QGraphicsPixmapItem *>(m_graphicsItem)
        ->setPixmap(this->getPixmap());
    m_onHitAnimationInProgress = false;
    m_onHitTimerId = -1;
  }
}

void EnemyShip::collideWith(GameObject &other) {
  other.collideWithEnemyShip(*this);
}

void EnemyShip::collideWithProjectile(Projectiles::Projectile &projectile) {
  this->takeDamage(projectile.getDamage());
  if (!this->isDestroyed())
    this->playOnHitAnimation();
}

void EnemyShip::collideWithEnemyShip(EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  this->takeDamage(0);
}

bool EnemyShip::shouldBeDeleted() {
  return m_destroyed || m_position.isBeyondScreenBottomLimit(30);
}

QPixmap EnemyShip::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

QPixmap EnemyShip::getOnHitPixmap() const {
  static QPixmap pixmap = GameObject::getOnHitPixmap();
  return pixmap;
}

} // namespace Ships

} // namespace GameObjects
