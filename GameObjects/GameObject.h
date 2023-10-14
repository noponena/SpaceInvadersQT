#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QGraphicsItem>
#include <unordered_set>
#include "Game/Movement/MovementStrategy.h"

namespace GameObjects {

class Projectile;
class EnemyShip;
class PlayerShip;

class Position {

public:
    Position(float x, float y, float minX=-1, float maxX=-1, float minY=-1, float maxY=-1)
        : pos(x, y),
          bounds(minX, minY, maxX - minX, maxY - minY) {}

    bool isBeyondScreenTopLimit(float offset = 0) const {
        return pos.y() + offset < bounds.top();
    }

    bool isBeyondScreenBottomLimit(float offset = 0) const {
        return pos.y() - offset > bounds.bottom();
    }

    bool isBeyondScreenLeftLimit(float offset = 0) const {
        return pos.x() + offset < bounds.left();
    }

    bool isBeyondScreenRightLimit(float offset = 0) const {
        return pos.x() - offset > bounds.right();
    }

    bool isBeyondAnyLimit() const {
        return isBeyondScreenTopLimit()
               || isBeyondScreenBottomLimit()
               || isBeyondScreenLeftLimit()
               || isBeyondScreenRightLimit();
    }

    float x() const { return pos.x(); }
    float y() const { return pos.y(); }

    void setX(float x) { pos.setX(x); }
    void setY(float y) { pos.setY(y); }

    void goToTopLimit() { pos.setY(bounds.top()); }
    void goToBottomLimit() { pos.setY(bounds.bottom()); }
    void goToLeftLimit() { pos.setX(bounds.left()); }
    void goToRightLimit() { pos.setX(bounds.right()); }

private:
    QPointF pos;
    QRectF bounds;  // Using QRectF to represent boundaries for more advanced manipulations if needed
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

    void update(float deltaTimeInSeconds);

    void moveLeft(float deltaTimeInSeconds);
    void moveRight(float deltaTimeInSeconds);

    void moveDown(float deltaTimeInSeconds);
    void moveUp(float deltaTimeInSeconds);

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
    void setMovementStrategy(const Game::Movement::MovementStrategy &newMovementStrategy);

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
            m_graphicsItem->setPos(m_position.x(), m_position.y());
        }
    }
    virtual void playOnDestructionAnimation() {};
private:
    inline void checkXConstraints();
    inline void checkYConstraints();
    inline void checkXYConstraints();
    inline void doMoveX(int amount);
    inline void doMoveY(int amount);

    int m_anchorX, m_anchorY, m_id;
    float m_currentSpeedX = 10;
    float m_currentSpeedY = 10;
    float m_acceleration = 1;
    float m_deceleration = 10;

    Game::Movement::MovementStrategy m_movementStrategy;
    void execMovement(float deltaTimeInSeconds);
    static int counter;
    std::unordered_set<int> m_collisions;
};
}

#endif // GAMEOBJECT_H
