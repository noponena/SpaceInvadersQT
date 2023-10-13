#ifndef GAMEOBJECTS_ENEMYSHIP_H
#define GAMEOBJECTS_ENEMYSHIP_H

#include "Ship.h"
#include "GameObjects/Projectiles/Projectile.h"
#include "GameObjects/Projectiles/Laser.h"

namespace GameObjects {

class EnemyShip : public Ship, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    EnemyShip(const int maxHp, int speed, int fireRate, const Position &position);
    void initialize() override;
    void shoot() override;
    void collideWith(GameObject &other) override;
    void collideWithProjectile(Projectile& projectile) override;
    void collideWithEnemyShip(EnemyShip& enemyShip) override;
    bool shouldBeDeleted() override;
protected:
    void playOnDestructionAnimation() override;
    void playOnHitAnimation();
    void timerEvent(QTimerEvent *event) override;
private:
    void switchToPixmapItem();
    int m_frameIndex;
    int m_onHitTimerId = -1;
    bool m_onHitAnimationInProgress = false;
    QColor m_originalColor;
};


} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
