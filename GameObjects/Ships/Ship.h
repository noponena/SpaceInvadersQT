#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/Effects/ParticleSystem.h"
#include "GameObjects/GameObject.h"
#include <QElapsedTimer>

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
  Ship(const int maxHp, int speed, const Position &position);
  virtual ~Ship();
  void shoot();
  bool shouldBeDeleted() override;
  void takeDamage(int amount);
  void heal(int amount);
  void updateFireRate(int amount = 1);

  void setWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);

  static QPixmap& loadSharedSpriteSheet(const QString &path) {
      static QPixmap m_sharedSpriteSheet;
      if (m_sharedSpriteSheet.isNull()) {
          m_sharedSpriteSheet.load(path);
      }
      return m_sharedSpriteSheet;
  }

  const magnetism &magnetism() const;

protected:
  int m_currentHp, m_maxHp, m_speed, m_fireRate, m_shotCooldownMs;
  int m_onHitTimerId = -1;
  std::unique_ptr<Weapons::Weapon> m_weapon;
  bool m_onHitAnimationInProgress = false;
  QList<QPixmap> m_animationFrames;
  QColor m_originalColor;
  QTimer *m_animationTimer;
  Effects::ParticleSystem *m_particleSystem;
  qreal m_halfWidth;
  qreal m_halfHeight;

  virtual bool isDestroyed() override;
  virtual void initializeDestructionAnimation() override;
  void playDestructionAnimation() override;
  void playDestructionEffects() override;

  virtual void playOnHitAnimation();
protected slots:
  void onAnimationCompleted();

private:
  int m_frameIndex;
  struct magnetism m_magnetism;

  void timerEvent(QTimerEvent *event) override;
signals:
  void animationCompleted();

  // GameObject interface
protected:
  void initializeDestructionEffects() override;
};
} // namespace Ships

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
