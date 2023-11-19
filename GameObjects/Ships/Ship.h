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
  Ship(const int maxHp, const float speed, const Position &position);
  virtual ~Ship();
  virtual void update(const UpdateContext &context) override;
  virtual bool shouldBeDeleted() override;
  bool isDead() override;
  virtual void takeDamage(int amount);
  virtual void heal(int amount);
  virtual void kill();
  virtual void restoreHealth();
  void firePrimaryWeapons();
  void fireActiveSecondaryWeapon();
  void updateFireRate(int amount = 1);
  void addPrimaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);
  void addSecondaryWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);
  void clearWeapons();

  int currentHp() const;
  void setImmortal(bool newImmortal);
  void setAutoShoot(bool newAutoShoot);

protected:
  bool m_immortal;
  int m_currentHp, m_maxHp, m_fireRate, m_fireCooldownMs, m_pixelWidth, m_pixelHeight, m_destructionParticleCount, m_activeSecondaryWeaponIndex;
  float m_speed;
  int m_onHitTimerId = -1;
  std::vector<std::unique_ptr<Weapons::Weapon>> m_primaryWeapons;
  std::vector<std::unique_ptr<Weapons::Weapon>> m_secondaryWeapons;
  bool m_onHitAnimationInProgress = false;
  bool m_autoShoot = false;
  QColor m_originalColor;

  virtual void initializeDestructionAnimation() override;
  void initializeDestructionEffects() override;

  virtual void playOnHitAnimation();
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
