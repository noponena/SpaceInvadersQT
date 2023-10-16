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

protected:
  void playOnHitAnimation() override;
  void timerEvent(QTimerEvent *event) override;
  QRectF getNonTransparentBoundingRect() override;
  QRectF boundingRect() const override;

  // GameObject interface
protected:
  QPointF getPixmapScaledSize() const override;
  QString getPixmapResourcePath() const override;
  QString getOnHitPixmapResourcePath() const override;
  QPixmap getPixmap() const override;
  QPixmap getOnHitPixmap() const override;
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
