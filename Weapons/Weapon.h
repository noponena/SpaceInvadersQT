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
  Weapon(float cooldownMs, Game::Movement::MovementStrategy movementStrategy)
      : m_owner(nullptr), m_cooldownMs(cooldownMs),
        m_movementStrategy(movementStrategy) {}
  virtual ~Weapon() = default;

  void shoot() {
    if (this->canShoot()) {
      std::shared_ptr<GameObjects::Projectiles::Projectile> projectile =
          this->createProjectile();
      projectile->initialize();
      projectile->setMovementStrategy(m_movementStrategy);
      emit projectileShot(projectile);
    }
  }

  virtual std::shared_ptr<GameObjects::Projectiles::Projectile>
  createProjectile() = 0;

  void setOwner(GameObjects::Ships::Ship *newOwner) { m_owner = newOwner; }

  void updateWeaponCooldown(float amount) { m_cooldownMs += amount; }

protected:
  GameObjects::Ships::Ship *m_owner;

private:
  float m_cooldownMs;
  QElapsedTimer m_lastShotTime;
  Game::Movement::MovementStrategy m_movementStrategy;

  bool canShoot() {
    int elapsed = m_lastShotTime.elapsed();
    if (elapsed < m_cooldownMs) {
      return false;
    }

    m_lastShotTime.restart();
    return true;
  }

signals:
  void projectileShot(
      const std::shared_ptr<GameObjects::Projectiles::Projectile> &projectile);
};

} // namespace Weapons

#endif // WEAPON_H
