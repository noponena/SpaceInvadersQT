#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QString>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <unordered_set>
#include "AnimatedGraphicsItem.h"
#include "Game/MovementStrategy.h"

namespace GameObjects {

class Projectile;
class EnemyShip;
class PlayerShip;

class Position {

public:
    Position(int x, int y, int minX=-1, int maxX=-1, int minY=-1, int maxY=-1)
        : x(x), y(y),
          minX(minX), minY(minY),
          maxX(maxX), maxY(maxY) {}

    bool isBeyondScreenTopLimit(int offset = 0) const {
        return this->y + offset < minY;
    }

    bool isBeyondScreenBottomLimit(int offset = 0) const {
        return this->y - offset > maxY;
    }

    bool isBeyondScreenLeftLimit(int offset = 0) const {
        return this->x + offset < minX;
    }

    bool isBeyondScreenRightLimit(int offset = 0) const {
        return this->x - offset > maxX;
    }

    bool isBeyondAnyLimit() const {
        return isBeyondScreenTopLimit()
               || isBeyondScreenBottomLimit()
               || isBeyondScreenLeftLimit()
               || isBeyondScreenRightLimit();
    }

    int x;
    int y;

    int minX;
    int minY;

    int maxX;
    int maxY;
};

class GameObject : public QObject {
    Q_OBJECT
public:
    GameObject(const Position &position, float speed);
    virtual ~GameObject() = default;
    virtual void initialize() = 0;
    virtual bool shouldBeDeleted() = 0;
    virtual void collideWith(GameObject &other) {(void)other;}
    virtual void collideWithProjectile(Projectile& projectile) {(void)projectile;}
    virtual void collideWithEnemyShip(EnemyShip& enemyShip) {(void)enemyShip;}
    virtual void collideWithPlayerShip(PlayerShip& playerShip) {(void)playerShip;}

    void update(int deltaTime);

    void moveLeft(int deltaTime);
    void moveRight(int deltaTime);

    void moveDown(int deltaTime);
    void moveUp(int deltaTime);

    void moveX(int amount);
    void moveY(int amount);

    bool isAtLimit() const;
    void doCollide(GameObject &other);
    bool isCollision(const GameObject &other) const;

    const Position &position() const;

    Position m_targetPosition;
    void lerpTowardsTarget(float t);
    QGraphicsItem *graphicsItem() const;
    void moveTo(const QPointF &newPosition);
    bool collidable() const;
    void setMovementStrategy(const Game::MovementStrategies::MovementStrategy &newMovementStrategy);

    int id();

protected:
    Position m_position;
    float m_speed;
    QGraphicsItem *m_graphicsItem;
    bool m_hasCollided;
    bool m_collidable;
    void clearMovementStrategy();
    inline void updateGraphicsItemPosition()
    {
        if (m_graphicsItem) {
            m_graphicsItem->setPos(m_position.x, m_position.y);
        }
    }
    virtual void playOnDestructionAnimation() {};
private:
    inline void checkXConstraints();
    inline void checkYConstraints();
    inline void checkXYConstraints();
    inline void doMoveX(int amount);
    inline void doMoveY(int amount);
    Game::MovementStrategies::MovementStrategy m_movementStrategy;
    void execMovement(int deltaTime);
    int m_id;
    static int counter;
    std::unordered_set<int> m_collisions;
};
}

#endif // GAMEOBJECT_H
