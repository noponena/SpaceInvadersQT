#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QString>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include "AnimatedGraphicsItem.h"

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

    bool isAtScreenTopLimit() const {
        return this->y == minY;
    }

    bool isAtScreenBottomLimit() const {
        return this->y == maxY;
    }

    bool isAtScreenLeftLimit() const {
        return this->x == minX;
    }

    bool isAtScreenRightLimit() const {
        return this->x == maxX;
    }

    bool isAtLimit() const {
        return isAtScreenTopLimit()
               || isAtScreenBottomLimit()
               || isAtScreenLeftLimit()
               || isAtScreenRightLimit();
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
    virtual void update(int deltaTime) = 0;
    virtual bool shouldBeDeleted() = 0;
    virtual void collideWith(GameObject &other) {(void)other;}
    virtual void collideWithProjectile(Projectile& projectile) {(void)projectile;}
    virtual void collideWithEnemyShip(EnemyShip& enemyShip) {(void)enemyShip;}
    virtual void collideWithPlayerShip(PlayerShip& playerShip) {(void)playerShip;}

    void moveLeft(int deltaTime);
    void moveRight(int deltaTime);

    void moveDown(int deltaTime);
    void moveUp(int deltaTime);

    void moveX(int amount);
    void moveY(int amount);

    bool isAtLimit() const;
    bool isCollision(GameObject &other);

    const Position &position() const;

    Position m_targetPosition;
    void lerpTowardsTarget(float t);
    QGraphicsItem *graphicsItem() const;

    void moveTo(const QPointF &newPosition);

    const BoundingBox &boundingBox() const;

    bool collidable() const;

protected:
    Position m_position;
    float m_speed;
    QGraphicsItem *m_graphicsItem;
    void calculateBoundingBox();
    bool m_hasCollided;
    bool m_collidable;
private:
    BoundingBox m_boundingBox;
};

}

#endif // GAMEOBJECT_H
