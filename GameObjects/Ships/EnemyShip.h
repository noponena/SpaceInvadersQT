#ifndef GAMEOBJECTS_ENEMYSHIP_H
#define GAMEOBJECTS_ENEMYSHIP_H

#include "GameObjects/Projectiles/Projectile.h"
#include "ShipWithHealthBar.h"

namespace GameObjects {
namespace Ships {
class EnemyShip : public ShipWithHealthBar {
  Q_OBJECT
public:
  EnemyShip(const int maxHp, const Position &position);
  void collideWith(GameObject &other) override;
  void collideWithProjectile(Projectiles::Projectile &projectile) override;
  void collideWithEnemyShip(EnemyShip &enemyShip) override;

  // GameObject interface
protected:
  QPixmap getPixmap() const override;
  QPixmap getOnHitPixmap() const override;
  void initiateDestructionProcedure() override;
  virtual void spawnCollectables(int amount);

  // GameObject interface
public:
  void update(const UpdateContext &context) override;
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
