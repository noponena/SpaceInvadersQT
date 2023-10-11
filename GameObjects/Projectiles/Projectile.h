#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "GameObjects/GameObject.h"

namespace GameObjects
{
class Projectile : public GameObject
{
public:
    Projectile(Position pos, float speed, bool hostile = false, int damage = 1);
    bool shouldBeDeleted() override;
    void collideWith(GameObject& other) override;
    void collideWithEnemyShip(EnemyShip& enemyShip) override;
    int getDamage() const;

    bool hostile() const;

protected:
    bool m_hostile;

private:
    int m_damage;
};
}



#endif // PROJECTILE_H