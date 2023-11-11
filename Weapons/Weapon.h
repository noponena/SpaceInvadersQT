#ifndef WEAPON_H
#define WEAPON_H

#include "GameObjects/Projectiles/Projectile.h"
#include "GameObjects/Ships/Ship.h"
#include <set>
#include <QElapsedTimer>

namespace GameObjects {} // namespace GameObjects

namespace Weapons {

enum class WeaponProperty {
    PIERCING,
    HOMING
};

class Weapon : public QObject {
  Q_OBJECT
public:
  Weapon(float cooldownMs, float minCooldownMs, Game::Movement::MovementStrategy movementStrategy, bool hostile = false, int damage = 1);
  virtual ~Weapon() = default;
  virtual GameObjects::Projectiles::Projectile*
  createProjectile() = 0;

  void shoot();
  void setOwner(GameObjects::Ships::Ship *newOwner);
  void updateWeaponCooldown(float amount);
  void enableSound();
  void disableSound();
  void addProperty(WeaponProperty property);
  void removeProperty(WeaponProperty property);

protected:
  GameObjects::Ships::Ship *m_owner;
    bool m_hostile;
    int m_damage;
    bool m_soundEnabled;
    std::set<WeaponProperty> m_properties;

private:
  float m_cooldownMs;
  float m_minCooldownMs;
  QElapsedTimer m_lastShotTimer;
  Game::Movement::MovementStrategy m_movementStrategy;

  bool canShoot();
  void clampCooldownMs();

signals:
  void projectileShot(
      GameObjects::Projectiles::Projectile *projectile);
};

} // namespace Weapons

#endif // WEAPON_H
