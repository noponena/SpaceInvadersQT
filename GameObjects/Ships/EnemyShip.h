#ifndef GAMEOBJECTS_ENEMYSHIP_H
#define GAMEOBJECTS_ENEMYSHIP_H

#include "Shooter.h"
#include "GameObjects/Projectiles/Projectile.h"
#include "GameObjects/Projectiles/Laser.h"

namespace GameObjects {

class EnemyShip : public Shooter
{
    Q_OBJECT
public:
    EnemyShip(const int maxHp, float speed, int fireRate, const Position &position);

    // GameObject interface
public:
    void initialize() override;
    bool shouldBeDeleted() override;

    // Shooter interface
public:
    void shoot() override;

    // GameObject interface
public:
    void collideWith(GameObject &other) override;
    void collideWithProjectile(Projectile& projectile) override;
    void playDestructionAnimation();
private:
    void switchToPixmapItem();
    QList<QPixmap> m_animationFrames;
    bool m_destroyed;
    int m_frameIndex;

private slots:
    void onAnimationCompleted();

signals:
    void animationCompleted();
};


} // namespace GameObjects

#endif // GAMEOBJECTS_ENEMYSHIP_H
