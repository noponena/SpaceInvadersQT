#include "EnemyShip.h"
#include "GameObjects/Effects/ParticleSystem.h"
#include <QColor>
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {
EnemyShip::EnemyShip(const int maxHp, int speed, const Position &position)
    : Ship(maxHp, speed, position) {}

void EnemyShip::initializeGraphicsItem() {
  QPixmap pixmap = getPixmap();

  // Create graphics item for the PNG image
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);

  // Assign the pixmapItem to m_graphicsItem
  m_graphicsItem = pixmapItem;
  m_nonTransparentBoundingRect = this->getNonTransparentBoundingRect();
  // There's no direct color for a QPixmap, but you can get a color using other
  // methods if required. The following line might not be applicable for a
  // pixmap: m_originalColor = static_cast<QGraphicsPolygonItem
  // *>(m_graphicsItem)->brush().color();
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

QRectF EnemyShip::boundingRect() const { return m_nonTransparentBoundingRect; }

QPointF EnemyShip::getPixmapScaledSize() const { return QPointF(50.0, 75.0); }

QString EnemyShip::getPixmapResourcePath() const {
  return QString(":/Images/alien.png");
}

QString EnemyShip::getOnHitPixmapResourcePath() const {
  return QString(":/Images/alien_on_hit.png");
}

QPixmap EnemyShip::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

QPixmap EnemyShip::getOnHitPixmap() const {
  static QPixmap pixmap = GameObject::getOnHitPixmap();
  return pixmap;
}

QRectF EnemyShip::getNonTransparentBoundingRect() {
  int minX = getPixmap().width();
  int minY = getPixmap().height();
  int maxX = 0;
  int maxY = 0;
  bool foundNonTransparent = false;

  QImage image = getPixmap().toImage();

  for (int y = 0; y < image.height(); ++y) {
    for (int x = 0; x < image.width(); ++x) {
      QColor pixelColor = image.pixelColor(x, y);
      if (pixelColor.alpha() > 0) { // or another threshold if needed
        foundNonTransparent = true;
        if (x < minX)
          minX = x;
        if (x > maxX)
          maxX = x;
        if (y < minY)
          minY = y;
        if (y > maxY)
          maxY = y;
      }
    }
  }

  if (foundNonTransparent) {
    return QRectF(minX, minY, maxX - minX + 1, maxY - minY + 1);
  } else {
    return QRectF();
  }
}
} // namespace Ships

} // namespace GameObjects
