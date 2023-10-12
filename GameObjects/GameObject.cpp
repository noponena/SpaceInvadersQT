#include "GameObject.h"
#include "Utils/Utils.h"
#include <cmath>

namespace GameObjects {

int GameObject::counter = 0;

GameObject::GameObject(const Position &position, float speed):
    m_targetPosition(0, 0), m_position(position), m_speed(speed),
    m_hasCollided(false), m_collidable(true)
{
    m_id = counter++;
}

void GameObject::update(int deltaTime)
{
    this->execMovement(deltaTime);
    this->updateGraphicsItemPosition();
}

void GameObject::moveLeft(int deltaTime)
{
    this->doMoveX(-std::round(m_speed * deltaTime));
}

void GameObject::moveRight(int deltaTime)
{
    this->doMoveX(std::round(m_speed * deltaTime));
}

void GameObject::moveDown(int deltaTime)
{
    this->doMoveY(std::round(m_speed * deltaTime));
}

void GameObject::moveUp(int deltaTime)
{
    this->doMoveY(-std::round(m_speed * deltaTime));
}

void GameObject::moveX(int amount)
{
    this->doMoveX(amount);
}

void GameObject::moveY(int amount)
{
    this->doMoveY(amount);
}

QGraphicsItem *GameObject::graphicsItem() const
{
    return m_graphicsItem;
}

void GameObject::clearMovementStrategy()
{
    m_movementStrategy.clear();
}

void GameObject::setMovementStrategy(const Game::MovementStrategies::MovementStrategy &newMovementStrategy)
{
    m_movementStrategy = newMovementStrategy;
}

void GameObject::execMovement(int deltaTime) {
    std::tuple<int, int> newPos = m_movementStrategy.move(m_position.x, m_position.y, deltaTime);
    m_position.x = std::get<0>(newPos);
    m_position.y = std::get<1>(newPos);
}

int GameObject::id()
{
    return m_id;
}

void GameObject::checkXConstraints()
{
    if (m_position.x > m_position.maxX)
        m_position.x = m_position.maxX;
    else if (m_position.x < m_position.minX)
        m_position.x = m_position.minX;
}

void GameObject::checkYConstraints()
{
    if (m_position.y > m_position.maxY)
        m_position.y = m_position.maxY;
    else if (m_position.y < m_position.minY)
        m_position.y = m_position.minY;
}

void GameObject::checkXYConstraints()
{
    this->checkXConstraints();
    this->checkYConstraints();
}

void GameObject::doMoveX(int amount)
{
    m_position.x += amount;
    this->checkXConstraints();
}

void GameObject::doMoveY(int amount)
{
    m_position.y += amount;
    this->checkYConstraints();
}


bool GameObject::collidable() const
{
    return m_collidable;
}

void GameObject::doCollide(GameObject& other)
{
    int local_id = this->id();
    int other_id = other.id();
    if (this->m_collisions.find(other_id) == this->m_collisions.end())
    {
        this->collideWith(other);
        m_collisions.insert(other_id);
    }

    if (other.m_collisions.find(local_id) == other.m_collisions.end())
    {
        other.collideWith(*this);
        other.m_collisions.insert(local_id);
    }

    //this->collideWith(other);
    //other.collideWith(*this);
}

bool GameObject::isCollision(const GameObject &other) const
{
    if (!this->collidable() || !other.collidable())
    {
        return false;
    }

    QRectF sceneRect1 = this->graphicsItem()->sceneBoundingRect();
    QRectF sceneRect2 = other.graphicsItem()->sceneBoundingRect();
    bool isCollision = sceneRect1.intersects(sceneRect2);

    return isCollision;
}

bool GameObject::isAtLimit() const
{
    return m_position.isBeyondAnyLimit();
}

const Position &GameObject::position() const
{
    return m_position;
}

}
