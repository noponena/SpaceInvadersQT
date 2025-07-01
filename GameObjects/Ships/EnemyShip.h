#ifndef GAMEOBJECTS_ENEMYSHIP_H
#define GAMEOBJECTS_ENEMYSHIP_H

#include "GameObjects/GameObjectBuilder.h"
#include "GameObjects/Projectiles/Projectile.h"
#include "ShipWithHealthBar.h"
#include "Weapons/WeaponBuilder.h"

namespace GameObjects {
namespace Ships {
class EnemyShip : public ShipWithHealthBar {
  Q_OBJECT
public:
  EnemyShip(const Config::GameContext &ctx);
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

  GameObjectBuilder m_gameObjectBuilder;
  Weapons::WeaponBuilder m_weaponBuilder;

private:
  bool m_bottomEdgeSignalEmitted;
  void clampHealthSpawnProbability();

  // GameObject interface
public:
  std::unique_ptr<GameObject> clone() const override;
  bool shouldBeDeleted() override;

signals:
  void enemyShipDeleted();
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
