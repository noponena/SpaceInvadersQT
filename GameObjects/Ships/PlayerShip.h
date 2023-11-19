#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "ShipWithHealthBar.h"

namespace GameObjects {
namespace Ships {
class PlayerShip : public ShipWithHealthBar {
  Q_OBJECT
public:
  PlayerShip(const int maxHp, const float speed, const Position &position);

  // GameObject interface
public:
  void collideWith(GameObject &other) override;
  void collideWithProjectile(Projectiles::Projectile &projectile) override;
  void collideWithCollectable(Collectables::Collectable &collectable) override;

  void accelerateLeft(float deltaTimeInSeconds);
  void accelerateRight(float deltaTimeInSeconds);
  void decelerateX(float deltaTimeInSeconds);
  void moveHorizontal(float deltaTimeInSeconds);
  void moveVertical(float deltaTimeInSeconds);
  void accelerateUp(float deltaTimeInSeconds);
  void accelerateDown(float deltaTimeInSeconds);
  void decelerateY(float deltaTimeInSeconds);

  inline void moveX(float amount);
  inline void moveY(float amount);

  // GameObject interface
protected:
  void disableMovement() override;

private:
  float m_currentSpeedX = 0;
  float m_currentSpeedY = 0;
  float m_acceleration = 1250;
signals:
  void stellarTokenCollected();

  // GameObject interface
protected:
  void initializeObjectType() override;
  void initializeSounds() override;
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
