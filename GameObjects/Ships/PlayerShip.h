#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "GameObjects/Projectiles/LaserBeam.h"
#include "Ship.h"

namespace GameObjects {
namespace Ships {
class PlayerShip : public Ship, public QGraphicsPolygonItem {
  Q_OBJECT
public:
  PlayerShip(const int maxHp, float speed, const Position &position);

  // GameObject interface
public:
  void collideWith(GameObject &other) override;
  void collideWithProjectile(Projectiles::Projectile &projectile) override;
  void collideWithStellarToken(Collectables::Stellar &stellarToken) override;

  // GameObject interface
protected:
  QPointF getPixmapScaledSize() const override;
  QString getPixmapResourcePath() const override;
  QString getOnHitPixmapResourcePath() const override;
  QPixmap getPixmap() const override;
signals:
  void stellarTokenCollected();
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
