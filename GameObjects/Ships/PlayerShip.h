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

  // GameObject interface
protected:
  void initializeGraphicsItem() override;
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
