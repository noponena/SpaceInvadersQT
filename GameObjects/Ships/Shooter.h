#ifndef GAMEOBJECTS_SHOOTER_H
#define GAMEOBJECTS_SHOOTER_H

#include <QElapsedTimer>

#include "Ship.h"

namespace GameObjects {

class Shooter : public Ship
{
public:
    Shooter(int maxHp, float speed, int fireRate, Position position);
    virtual void shoot() = 0;
    void updateFireRate(int amount = 1);
    int fireRate() const;

protected:
    int m_fireRate;
    bool canShoot();
    QElapsedTimer m_lastShotTime;
    int m_shotCooldown;  // Time in milliseconds between shots
};

} // namespace GameObjects

#endif // GAMEOBJECTS_SHOOTER_H
