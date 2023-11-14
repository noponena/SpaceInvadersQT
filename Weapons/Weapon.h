#ifndef WEAPON_H
#define WEAPON_H

#include "GameObjects/Projectiles/Projectile.h"
#include "GameObjects/Ships/Ship.h"
#include <QElapsedTimer>
#include <set>

namespace GameObjects {} // namespace GameObjects

namespace Weapons {

enum class ProjectileProperty { PIERCING, HOMING };

class Weapon : public QObject {
  Q_OBJECT
public:
  Weapon();

  virtual ~Weapon() = default;
  virtual std::unique_ptr<GameObjects::Projectiles::Projectile>
  createProjectile() = 0;
  virtual std::unique_ptr<Weapon> clone() const = 0;

  void setProjectilePrototype(
      std::unique_ptr<GameObjects::Projectiles::Projectile> prototype);
  void shoot();
  void setOwner(GameObjects::Ships::Ship *newOwner);
  void updateWeaponCooldown(float amount);
  void enableSound();
  void disableSound();
  void addProjectileProperty(ProjectileProperty property);
  void removeProjectileProperty(ProjectileProperty property);

  void setCooldownMs(float newCooldownMs);

protected:
  GameObjects::Ships::Ship *m_owner;
  bool m_soundEnabled;
  float m_cooldownMs;
  std::unique_ptr<GameObjects::Projectiles::Projectile> m_projectilePrototype;

private:
  float m_minCooldownMs;
  QElapsedTimer m_lastShotTimer;

  bool canShoot();
  void clampCooldownMs();

signals:
  void projectileShot(
      std::shared_ptr<GameObjects::Projectiles::Projectile> projectile);
};

} // namespace Weapons

#endif // WEAPON_H
