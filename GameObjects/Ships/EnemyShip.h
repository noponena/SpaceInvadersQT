#ifndef GAMEOBJECTS_ENEMYSHIP_H
#define GAMEOBJECTS_ENEMYSHIP_H

#include "Shooter.h"
#include "GameObjects/Projectiles/Projectile.h"
#include "GameObjects/Projectiles/Laser.h"

namespace GameObjects {

class EnemyShip : public Shooter, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    EnemyShip(const int maxHp, float speed, int fireRate, const Position &position);

    // GameObject interface
public:
    void initialize() override;

    // Shooter interface
public:
    void shoot() override;

    // GameObject interface
public:
    void collideWith(GameObject &other) override;
    void collideWithProjectile(Projectile& projectile) override;
    void collideWithEnemyShip(EnemyShip& enemyShip) override;
protected:
    void playOnDestructionAnimation() override;
    void playOnHitAnimation();
    void timerEvent(QTimerEvent *event) override;
private:
    void switchToPixmapItem();
    int m_frameIndex;
    int m_oscillationAmplitude = 5;
    float m_oscillationFrequency = 1;
    bool m_onHitAnimationInProgress = false;
    int m_onHitTimerId = -1;
    QColor m_originalColor;

    // GameObject interface
public:
    bool shouldBeDeleted() override;
};


} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
