#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"

namespace GameObjects {
namespace Ships {
class PlayerShip : public Ship {
  Q_OBJECT
public:
  PlayerShip(const Config::GameContext &ctx);

  // GameObject interface
public:
  void update(const UpdateContext &context) override;
  virtual bool shouldBeDeleted() override;

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
                          std::uint32_t weaponIndex) override;
  bool fireSecondaryWeapon(std::uint32_t weaponIndex) override;

  inline void moveRelativeX(float displacement);
  inline void moveRelativeY(float displacement);

  float maxEnergy() const;
  void setMaxEnergy(float newMaxEnergy);

  float maxHealth() const;
  void setMaxHealth(float newMaxHealth);

  void setSpeed(float newSpeed);

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
  void playerSecondaryWeaponFired(std::uint32_t weaponIndex,
                                  std::uint32_t cooldownMs);
  void healthUpdated(int amount);
  void playerEnergyUpdated(float updatedEnergy);
  void playerMaxEnergySet(float maxEnergy);
  void playerHealthUpdated(int updatedHealth);
  void playerMaxHealthSet(int maxHealth);

  // GameObject interface
protected:
  void initializeObjectType() override;
  void initializeSounds() override;

  // GameObject interface
public:
  std::unique_ptr<GameObject> clone() const override;
};
} // namespace Ships

} // namespace GameObjects

#endif // PLAYERSHIP_H
