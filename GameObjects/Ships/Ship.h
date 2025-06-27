#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/AttractableGameObject.h"
#include "Utils/BoundsChecker.h"
#include "Weapons/Weapon.h"
#include <QElapsedTimer>
#include <QTimer>

namespace Weapons {
class Weapon;
}

namespace GameObjects {
namespace Ships {

class Ship : public AttractableGameObject {
  Q_OBJECT
public:
  Ship(const std::uint32_t maxHp, const float speed, const Transform &transform,
       const Config::GameContext ctx);
  virtual ~Ship();
  virtual void update(const UpdateContext &context) override;
  virtual bool shouldBeDeleted() override;
  bool isDead() override;
  virtual void takeDamage(std::uint32_t amount);
  virtual void heal(std::uint32_t amount);
  virtual void kill();
  virtual void restoreHealth();
  void firePrimaryWeapons();
  virtual bool fireSecondaryWeapon(std::uint32_t weaponIndex);
  void updateFireRate(int amount = 1);
  void addPrimaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);
  virtual void setSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon,
                                  std::uint32_t weaponIndex);
  void clearWeapons();

  int currentHp() const;
  void setImmortal(bool newImmortal);
  void setAutoShoot(bool newAutoShoot);

  void setDestructionParticleCount(int newDestructionParticleCount);
  void fullyRestoreEnergy();
  void fullyRestoreHealth();

  std::uint32_t energyRegenerationRate() const;

  void setEnergyRegenerationRate(std::uint32_t newEnergyRegenerationRate);
  void setMaxHealth(float newMaxHealth);
  void setSpeed(float newSpeed);
  void setMaxEnergy(float newMaxEnergy);

protected:
  bool m_immortal;
  std::uint32_t m_fireCooldownMs, m_pixelWidth, m_pixelHeight,
      m_destructionParticleCount;
  float m_currentHealth, m_maxHealth, m_speed, m_maxEnergy, m_currentEnergy,
      m_energyRegenerationRate;
  int m_onHitTimerId = -1;
  std::vector<std::unique_ptr<Weapons::Weapon>> m_primaryWeapons;
  std::unique_ptr<Weapons::Weapon> m_secondaryWeapons[4];
  bool m_onHitAnimationInProgress = false;
  bool m_autoFire = false;
  QColor m_originalColor;

  virtual void initializeDestructionAnimation() override;
  void initializeDestructionEffects() override;
  virtual void playOnHitAnimation();
  void regenerateEnergy(float deltaTimeInSeconds);
protected slots:
  void onProjectileFired(
      std::shared_ptr<GameObjects::Projectiles::Projectile> projectile);

private:
  void timerEvent(QTimerEvent *event) override;

signals:
  void bottomEdgeReached();

  // GameObject interface
protected:
  void initializeObjectType() override;
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
