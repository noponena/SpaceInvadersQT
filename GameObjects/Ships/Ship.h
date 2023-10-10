#ifndef GAMEOBJECTS_SHIP_H
#define GAMEOBJECTS_SHIP_H

#include "GameObjects/GameObject.h"

namespace GameObjects {

class Ship : public GameObject
{
    Q_OBJECT
public:
    Ship(const int maxHp, float speed, const Position &position);
    void takeDamage(int amount);
    void heal(int amount);
    bool isAlive();

protected:
    int m_currentHp;
    int m_maxHp;
    float m_speed;
    bool m_destroyed;
    QList<QPixmap> m_animationFrames;
    virtual void initializeDestructionAnimation();

private:
    void die();

protected slots:
    void onAnimationCompleted();

signals:
    void animationCompleted();

    // GameObject interface
public:
    void initialize() override;
    bool shouldBeDeleted() override;
};

} // namespace GameObjects

#endif // GAMEOBJECTS_SHIP_H
