#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QGraphicsItem>
#include <unordered_set>
#include "Position.h"
#include "Game/Movement/MovementStrategy.h"

namespace GameObjects {

namespace Projectiles { class Projectile; }
namespace Ships
{
class EnemyShip;
class PlayerShip;
}


class GameObject : public QObject {
    Q_OBJECT

public:
    // Constructors & Destructor
    GameObject(const Position &position, float speed);
    virtual ~GameObject() = default;

    // Initialization & Lifecycle
    void initialize();
    virtual bool shouldBeDeleted() = 0;

    // Collision Handlers
    virtual void collideWith(GameObject &other) { (void)other; }
    virtual void collideWithProjectile(Projectiles::Projectile& projectile) { (void)projectile; }
    virtual void collideWithEnemyShip(Ships::EnemyShip& enemyShip) { (void)enemyShip; }
    virtual void collideWithPlayerShip(Ships::PlayerShip& playerShip) { (void)playerShip; }

    // Update & Movement
    void update(float deltaTimeInSeconds);
    void moveX(float amount);
    void moveY(float amount);
    void accelerateLeft(float deltaTimeInSeconds);
    void accelerateRight(float deltaTimeInSeconds);
    void decelerateX(float deltaTimeInSeconds);
    void moveHorizontal(float deltaTimeInSeconds);
    void moveVertical(float deltaTimeInSeconds);
    void accelerateUp(float deltaTimeInSeconds);
    void accelerateDown(float deltaTimeInSeconds);
    void decelerateY(float deltaTimeInSeconds);

    // Getters & Setters
    const Position &getPosition() const;
    QGraphicsItem *getGraphicsItem() const;
    bool isCollidable() const;
    int id();

    // Actions & Modifiers
    void moveTo(const QPointF &newPosition);
    void setMovementStrategy(const Game::Movement::MovementStrategy &newMovementStrategy);
    void collide(GameObject &other);
    bool isAtLimit() const;
    bool isCollidingWith(const GameObject &other) const;

protected:
    // Member Variables
    Position m_position;
    float m_speed;
    QGraphicsItem *m_graphicsItem;
    bool m_hasCollided;
    bool m_collidable;

    // Protected Helpers & Methods
    void clearMovementStrategy();
    virtual void initializeGraphicsItem() = 0;
    virtual void initializeDestructionAnimation() {};
    virtual void initializeDestructionEffects() {};
    virtual void playDestructionAnimation() {};
    virtual void playDestructionEffects() {};
    virtual bool isDestroyed() { return false; };
    inline void updateGraphicsItemPosition() {
        if (m_graphicsItem) {
            m_graphicsItem->setPos(m_position.pos);
        }
    }

private:
    // Member Variables
    int m_id;
    float m_currentSpeedX = 0;
    float m_currentSpeedY = 0;
    float m_acceleration = 1250;
    bool m_destructionInitiated;
    Game::Movement::MovementStrategy m_movementStrategy;
    static int counter;
    std::unordered_set<int> m_collisions;

    // Private Helpers & Methods
    inline void clampToXBounds();
    inline void clampToYBounds();
    inline void clampToXYBounds();
    inline void doMoveX(float amount);
    inline void doMoveY(float amount);
    void applyMovementStrategy(float deltaTimeInSeconds);
    void initiateDestructionProcedure();
};

}

#endif // GAMEOBJECT_H
