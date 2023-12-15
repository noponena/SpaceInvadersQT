#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"

namespace GameObjects {
namespace Ships {
class PlayerShip : public Ship {
  Q_OBJECT
public:
  PlayerShip(const float speed, const Position &position);

  // GameObject interface
public:
  void update(const UpdateContext &context) override;

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

  void setSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon,
                          unsigned weaponIndex) override;
  bool fireSecondaryWeapon(unsigned int weaponIndex) override;

  inline void moveX(float amount);
  inline void moveY(float amount);

  float maxEnergy() const;
  void setMaxEnergy(float newMaxEnergy);

  float maxHealth() const;
  void setMaxHealth(float newMaxHealth);

  // GameObject interface
protected:
  void disableMovement() override;

private:
  float m_currentSpeedX = 0;
  float m_currentSpeedY = 0;
  float m_acceleration = 1250;
signals:
  void stellarTokenCollected();
  void
  playerSecondaryWeaponsChanged(std::unique_ptr<Weapons::Weapon> weapons[]);
  void playerSecondaryWeaponFired(unsigned weaponIndex, unsigned cooldownMs);
  void healthUpdated(int amount);
  void playerEnergyUpdated(float updatedEnergy);
  void playerMaxEnergySet(float maxEnergy);
  void playerHealthUpdated(int updatedHealth);
  void playerMaxHealthSet(int maxHealth);

  // GameObject interface
protected:
  void initializeObjectType() override;
  void initializeSounds() override;
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
