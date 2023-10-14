#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Effects/ParticleSystem.h"
#include "Weapons/Weapon.h"
#include <QElapsedTimer>

namespace GameObjects {
namespace Ships {
class Ship : public GameObject
{
    Q_OBJECT
public:
    Ship(const int maxHp, int speed, const Position &position);
    virtual ~Ship() = default;
    void shoot();
    void initialize() override;
    bool shouldBeDeleted() override;
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive();
    void updateFireRate(int amount = 1);

    void setWeapon(std::unique_ptr<Weapons::Weapon> newWeapon);

protected:
    int m_currentHp, m_maxHp, m_speed, m_fireRate, m_shotCooldownMs;
    std::unique_ptr<Weapons::Weapon> m_weapon;
    bool m_destroyed;
    QList<QPixmap> m_animationFrames;
    QElapsedTimer m_lastShotTime;

    virtual void initializeDestructionAnimation();

private:
    void die();

protected slots:
    void onAnimationCompleted();

signals:
    void animationCompleted();
    void projectileShot(const std::shared_ptr<Projectiles::Projectile>& projectile);

};
}


} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
