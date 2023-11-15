#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"
#include "HealthBar.h"

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
  QPixmap getPixmap() const override;
  void disableMovement() override;

private:
  float m_speed;
  float m_currentSpeedX = 0;
  float m_currentSpeedY = 0;
  float m_acceleration = 1250;
  std::unique_ptr<HealthBar> m_healthBar;
signals:
  void stellarTokenCollected();
  void playerShipDestroyed();

  // GameObject interface
  public:
  void initialize() override;

      // Ship interface
  public:
      void takeDamage(int amount) override;
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
