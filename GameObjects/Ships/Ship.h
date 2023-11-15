#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/Animations/AnimatedItem.h"
#include "GameObjects/Effects/ParticleSystem.h"
#include "GameObjects/GameObject.h"
#include <QElapsedTimer>
#include <QTimer>

namespace Weapons {
class Weapon;
}

namespace GameObjects {
namespace Ships {
struct magnetism {
  bool isMagnetic;
  float magneticRadius;
  float magneticStrength;
};
class Ship : public GameObject {
  Q_OBJECT
public:
  Ship(const int maxHp, const Position &position);
  virtual ~Ship();
  void shoot();
  bool shouldBeDeleted() override;
  bool isDead() override;
  virtual void takeDamage(int amount);
  void heal(int amount);
  void updateFireRate(int amount = 1);
  void addWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);

  static QPixmap &loadSharedSpriteSheet(const QString &path) {
    static QPixmap m_sharedSpriteSheet;
    if (m_sharedSpriteSheet.isNull()) {
      m_sharedSpriteSheet.load(path);
    }
    return m_sharedSpriteSheet;
  }

  const magnetism &magnetism() const;

  int currentHp() const;

protected:
  int m_currentHp, m_maxHp, m_fireRate, m_shotCooldownMs;
  int m_onHitTimerId = -1;
  std::vector<std::unique_ptr<Weapons::Weapon>> m_primaryWeapons;
  bool m_onHitAnimationInProgress = false;
  QColor m_originalColor;
  Effects::ParticleSystem m_destructionEffect;
  Animations::AnimatedItem m_destructionAnimation;
  qreal m_halfWidth;
  qreal m_halfHeight;
  QPixmap m_sharedPixmap;

  virtual void initializeDestructionAnimation() override;
  void playDestructionAnimation() override;
  void playDestructionEffects() override;
  void initializeDestructionEffects() override;

  virtual void playOnHitAnimation();
protected slots:
  void onProjectileShot(std::shared_ptr<Projectiles::Projectile> projectile);

private:
  struct magnetism m_magnetism;

  void timerEvent(QTimerEvent *event) override;
signals:
  void destructionCompleted();
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
