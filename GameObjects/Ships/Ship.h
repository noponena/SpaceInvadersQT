#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/GameObject.h"
#include <QElapsedTimer>
#include <QTimer>

namespace Weapons {
class Weapon;
}

namespace GameObjects {
namespace Ships {
struct Magnetism {
  bool isMagnetic;
  float magneticRadius;
  float magneticStrength;
};
class Ship : public GameObject {
  Q_OBJECT
public:
  Ship(const int maxHp, const float speed, const Position &position);
  virtual ~Ship();
  virtual void takeDamage(int amount);
  virtual void heal(int amount);
  void shoot();
  bool shouldBeDeleted() override;
  bool isDead() override;
  void updateFireRate(int amount = 1);
  void addWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);
  void clearWeapons();

  static QPixmap &loadSharedSpriteSheet(const QString &path) {
    static QPixmap m_sharedSpriteSheet;
    if (m_sharedSpriteSheet.isNull()) {
      m_sharedSpriteSheet.load(path);
    }
    return m_sharedSpriteSheet;
  }

  const Magnetism &magnetism() const;

  int currentHp() const;

  void setImmortal(bool newImmortal);

  void setAutoShoot(bool newAutoShoot);

protected:
  bool m_immortal;
  int m_currentHp, m_maxHp, m_fireRate, m_shotCooldownMs;
  float m_speed;
  int m_onHitTimerId = -1;
  std::vector<std::unique_ptr<Weapons::Weapon>> m_primaryWeapons;
  bool m_onHitAnimationInProgress = false;
  bool m_autoShoot = false;
  QColor m_originalColor;
  QPixmap m_sharedPixmap;
  struct Magnetism m_magnetism;

  virtual void initializeDestructionAnimation() override;
  void initializeDestructionEffects() override;

  virtual void playOnHitAnimation();
protected slots:
  void onProjectileShot(std::shared_ptr<Projectiles::Projectile> projectile);

private:
  void timerEvent(QTimerEvent *event) override;

  // GameObject interface
public:
  void update(const UpdateContext &context) override;
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
