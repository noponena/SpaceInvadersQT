#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QString>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include "AnimatedGraphicsItem.h"
#include "Game/MovementFunction.h"

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

    std::tuple<int, int> get() const {
        return std::make_tuple(x, y);
    }

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

    bool isBeyondLimit() const {
        return isBeyondScreenTopLimit()
               || isBeyondScreenBottomLimit()
               || isBeyondScreenLeftLimit()
               || isBeyondScreenRightLimit();
    }

    void moveX(int amount) {
        this->x += amount;
        if (this->x > maxX)
            this->x = maxX;
        else if (this->x < minX)
            this->x = minX;
    }

    void moveY(int amount) {
        this->y += amount;
        if (this->y > maxY)
            this->y = maxY;
        else if (this->y < minY)
            this->y = minY;
    }


    bool operator==(const Position& other) const {
        return this->x == other.x && this->y == other.y;
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    void setX(int newX)
    {
        if (newX > maxX)
            x = maxX;
        else if (newX < minX)
            x = minX;
        else
            x = newX;
    }

    void setY(int newY)
    {
        y = newY;
    }

private:
    int x;
    int y;

    int minX;
    int minY;

    int maxX;
    int maxY;
};

class GameObject : public QObject {
    Q_OBJECT
    //Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    struct BoundingBox {
        int minX, minY, maxX, maxY;
    };

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
    bool checkCollision(GameObject &other);

    const Position &position() const;

    Position m_targetPosition;
    void lerpTowardsTarget(float t);
    QGraphicsItem *graphicsItem() const;

    void moveTo(const QPointF &newPosition);

    const BoundingBox &boundingBox() const;

    bool collidable() const;

    void setXMovementFunc(const MovementFunction::MovementFunc &newMovementFunc);
    void setYMovementFunc(const MovementFunction::MovementFunc &newYMovementFunc);

protected:
    Position m_position;
    float m_speed;
    QGraphicsItem *m_graphicsItem;
    void updateBoundingBox();
    bool m_hasCollided;
    bool m_collidable;


    virtual void playDestructionAnimation() {};
private:
    BoundingBox m_boundingBox;
    void move(int xRel, int yRel);
    MovementFunction::MovementFunc m_xMovementFunc;
    MovementFunction::MovementFunc m_yMovementFunc;
    void execMovement(int deltaTime);
    void updateGraphicsItemPosition();
};

}

#endif // GAMEOBJECT_H
