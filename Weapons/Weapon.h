#ifndef WEAPON_H
#define WEAPON_H

#include "GameObjects/Projectiles/Projectile.h"
#include <QElapsedTimer>

namespace GameObjects {
namespace Ships {
class Ship;
}
} // namespace GameObjects

namespace Weapons {

class Weapon : public QObject {
  Q_OBJECT
public:
  Weapon();

  virtual ~Weapon() = default;
  virtual std::unique_ptr<Weapon> clone() const = 0;
  virtual QString getHudImagePath() const = 0;

  void setProjectilePrototype(
      std::unique_ptr<GameObjects::Projectiles::Projectile> prototype);
  bool fire();
  void setOwner(GameObjects::Ships::Ship *newOwner);
  void updateWeaponCooldown(float amount);
  void enableSound();
  void disableSound();
  void
  addProjectileProperty(GameObjects::Projectiles::ProjectileProperty property);
  void removeProjectileProperty(
      GameObjects::Projectiles::ProjectileProperty property);

  void setCooldownMs(float newCooldownMs);

  GameObjects::Projectiles::Projectile *projectilePrototype() const;

  float cooldownMs() const;

  std::uint32_t energyConsuption() const;
  void setEnergyConsuption(std::uint32_t newEnergyConsuption);

protected:
  GameObjects::Ships::Ship *m_owner;
  bool m_soundEnabled;
  float m_cooldownMs;
  std::unique_ptr<GameObjects::Projectiles::Projectile> m_projectilePrototype;

private:
  std::uint32_t m_energyConsuption;
  float m_minCooldownMs;
  QElapsedTimer m_lastFiredTimer;
  bool m_firstShot;

  std::unique_ptr<GameObjects::Projectiles::Projectile> createProjectile();
  bool canFire();
  void clampCooldownMs();

signals:
  void projectileFired(
      std::shared_ptr<GameObjects::Projectiles::Projectile> projectile);
};

} // namespace Weapons

#endif // WEAPON_H
