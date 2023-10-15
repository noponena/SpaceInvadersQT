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
  // Create graphics item
  QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();

  // Set the color and pen properties
  polygonItem->setBrush(Qt::green);
  polygonItem->setPen(Qt::NoPen);

  // Create a QPolygon to represent the triangle and set it to the
  // QGraphicsPolygonItem
  QPolygon triangle;
  triangle << QPoint(0, 10) << QPoint(-10, -20)
           << QPoint(10, -20); // Assuming the ship is centered at (0, 0)
  polygonItem->setPolygon(triangle);

  // Assign the polygonItem to m_graphicsItem
  m_graphicsItem = polygonItem;
  m_originalColor =
      static_cast<QGraphicsPolygonItem *>(m_graphicsItem)->brush().color();
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
} // namespace Ships

} // namespace GameObjects
