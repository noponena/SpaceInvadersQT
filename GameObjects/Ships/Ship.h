#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/GameObject.h"

namespace GameObjects {

class Ship : public GameObject
{
public:
    Ship(const int maxHp, float speed, const Position &position);
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive();

protected:
    int m_currentHp;
    int m_maxHp;
    float m_speed;
};

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
