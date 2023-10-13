#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"
#include "GameObjects/Projectiles/Laser.h"

namespace GameObjects {

class PlayerShip : public Ship, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    PlayerShip(const int maxHp, float speed, int fireRate, const Position &position);

    // GameObject interface
public:
    void initialize() override;
    void shoot() override;
    void collideWith(GameObject &other) override;
    void collideWithProjectile(Projectile& projectile) override;
};

}


#endif // PLAYERSHIP_H
