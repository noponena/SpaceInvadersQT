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

  void setStellarCoinSpawnRange(QPoint newStellarCoinSpawnRange);
  void setHealthSpawnProbability(float newHealthSpawnProbability);
  void updateHealthSpawnProbability(float multiplier);


protected:
  void executeDestructionProcedure() override;
  void spawnStellarCoins();
  void spawnHealth();

  // GameObject interface
protected:
  QPoint m_stellarCoinSpawnRange;
  float m_healthSpawnProbability;

  void initializeObjectType() override;
  void initializeSounds() override;
private:
  void clampHealthSpawnProbability();
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
