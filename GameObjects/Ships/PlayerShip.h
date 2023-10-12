#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Shooter.h"
#include "GameObjects/Projectiles/Laser.h"
#include <vector>

namespace GameObjects {

class PlayerShip : public Shooter, public QGraphicsPolygonItem
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

signals:
    void laserShot(const std::shared_ptr<Laser>& laser);
};

}


#endif // PLAYERSHIP_H
