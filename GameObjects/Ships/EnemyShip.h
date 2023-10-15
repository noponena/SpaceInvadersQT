#ifndef GAMEOBJECTS_ENEMYSHIP_H
#define GAMEOBJECTS_ENEMYSHIP_H

#include "GameObjects/Projectiles/LaserBeam.h"
#include "GameObjects/Projectiles/Projectile.h"
#include "Ship.h"

namespace GameObjects {
namespace Ships {
class EnemyShip : public Ship, public QGraphicsPolygonItem {
  Q_OBJECT
public:
  EnemyShip(const int maxHp, int speed, const Position &position);
  void collideWith(GameObject &other) override;
  void collideWithProjectile(Projectiles::Projectile &projectile) override;
  void collideWithEnemyShip(EnemyShip &enemyShip) override;
  bool shouldBeDeleted() override;

  // GameObject interface
protected:
  void initializeGraphicsItem() override;
private:

  // Static function to get the shared pixmap
  static const QPixmap& getPixmap() {
      static QPixmap sharedPixmap(":/Images/alien.png");
      qreal width = 50.0;
      qreal height = 75.0;
      sharedPixmap = sharedPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      return sharedPixmap;
  }

  // Static function to get the shared pixmap
  static const QPixmap& getOnHitPixmap() {
      static QPixmap sharedPixmap(":/Images/alien_on_hit.png");
      qreal width = 50.0;
      qreal height = 75.0;
      sharedPixmap = sharedPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      return sharedPixmap;
  }

protected:
  void playOnHitAnimation() override;
  void timerEvent(QTimerEvent *event) override;
  QRectF getNonTransparentBoundingRect() override;
  QRectF boundingRect() const override;
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
