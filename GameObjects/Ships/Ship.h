#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Effects/ParticleSystem.h"
#include <QElapsedTimer>

namespace GameObjects {

class Ship : public GameObject
{
    Q_OBJECT
public:
    Ship(const int maxHp, int speed, int fireRate, const Position &position);
    virtual void shoot() = 0;
    void initialize() override;
    bool shouldBeDeleted() override;
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive();
    void updateFireRate(int amount = 1);
    int fireRate() const;

protected:
    int m_currentHp, m_maxHp, m_speed, m_fireRate, m_shotCooldownMs;
    bool m_destroyed;
    QList<QPixmap> m_animationFrames;
    QElapsedTimer m_lastShotTime;

    virtual void initializeDestructionAnimation();
    bool canShoot();

private:
    void die();

protected slots:
    void onAnimationCompleted();

signals:
    void animationCompleted();
    void projectileShot(const std::shared_ptr<Projectile>& projectile);

};

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
