#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/Effects/ParticleSystem.h"
#include "GameObjects/GameObject.h"
#include "Weapons/Weapon.h"
#include <QElapsedTimer>

namespace GameObjects {
namespace Ships {
class Ship : public GameObject {
  Q_OBJECT
public:
  Ship(const int maxHp, int speed, const Position &position);
  virtual ~Ship() = default;
  void shoot();
  bool shouldBeDeleted() override;
  void takeDamage(int amount);
  void heal(int amount);
  void updateFireRate(int amount = 1);

  void setWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);

protected:
  int m_currentHp, m_maxHp, m_speed, m_fireRate, m_shotCooldownMs;
  std::unique_ptr<Weapons::Weapon> m_weapon;
  bool m_destroyed;
  QList<QPixmap> m_animationFrames;
  QElapsedTimer m_lastShotTime;
  QColor m_originalColor;

  virtual bool isDestroyed() override;
  virtual void initializeDestructionAnimation() override;
  void playDestructionAnimation() override;
  void playDestructionEffects() override;

  void playOnHitAnimation();
protected slots:
  void onAnimationCompleted();

private:
  int m_onHitTimerId = -1;
  void switchToPixmapItem();
  int m_frameIndex;
  bool m_onHitAnimationInProgress = false;

  void timerEvent(QTimerEvent *event) override;
signals:
  void animationCompleted();
  void
  projectileShot(const std::shared_ptr<Projectiles::Projectile> &projectile);
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
