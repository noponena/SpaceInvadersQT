#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/AttractableGameObject.h"
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
  Ship(const unsigned maxHp, const float speed, const Position &position);
  virtual ~Ship();
  virtual void update(const UpdateContext &context) override;
  virtual bool shouldBeDeleted() override;
  bool isDead() override;
  virtual void takeDamage(unsigned int amount);
  virtual void heal(unsigned int amount);
  virtual void kill();
  virtual void restoreHealth();
  void firePrimaryWeapons();
  virtual bool fireSecondaryWeapon(unsigned int weaponIndex);
  void updateFireRate(int amount = 1);
  void addPrimaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);
  virtual void setSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon,
                                  unsigned weaponIndex);
  void clearWeapons();

  int currentHp() const;
  void setImmortal(bool newImmortal);
  void setAutoShoot(bool newAutoShoot);

  void setDestructionParticleCount(int newDestructionParticleCount);
  void fullyRestoreEnergy();
  void fullyRestoreHealth();

  unsigned int energyRegenerationRate() const;
  void setEnergyRegenerationRate(unsigned int newEnergyRegenerationRate);

protected:
  bool m_immortal;
  unsigned m_fireCooldownMs, m_pixelWidth, m_pixelHeight,
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
  void onProjectileFired(std::shared_ptr<Projectiles::Projectile> projectile);

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
