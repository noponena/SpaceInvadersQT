#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"

namespace GameObjects {
namespace Ships {
class PlayerShip : public Ship {
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
  QPixmap getPixmap() const override;
signals:
  void stellarTokenCollected();

    // GameObject interface
public:
    bool shouldBeDeleted() override;
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
