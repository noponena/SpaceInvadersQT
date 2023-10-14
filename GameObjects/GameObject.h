#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QGraphicsItem>
#include <unordered_set>
#include "Position.h"
#include "Game/Movement/MovementStrategy.h"

namespace GameObjects {

class Projectile;
class EnemyShip;
class PlayerShip;

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

    void moveX(float amount);
    void moveY(float amount);

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

    void accelerateLeft(float deltaTimeInSeconds);
    void accelerateRight(float deltaTimeInSeconds);
    void decelerateX(float deltaTimeInSeconds);
    void moveHorizontal(float deltaTimeInSeconds);
    void moveVertical(float deltaTimeInSeconds);
    void accelerateUp(float deltaTimeInSeconds);
    void accelerateDown(float deltaTimeInSeconds);
    void decelerateY(float deltaTimeInSeconds);
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
            m_graphicsItem->setPos(m_position.pos);
        }
    }
    virtual void playOnDestructionAnimation() {};
private:
    inline void checkXConstraints();
    inline void checkYConstraints();
    inline void checkXYConstraints();
    inline void doMoveX(float amount);
    inline void doMoveY(float amount);

    int m_id;
    float m_currentSpeedX = 0;
    float m_currentSpeedY = 0;
    float m_acceleration = 1250;

    Game::Movement::MovementStrategy m_movementStrategy;
    void execMovement(float deltaTimeInSeconds);
    static int counter;
    std::unordered_set<int> m_collisions;
};
}

#endif // GAMEOBJECT_H
