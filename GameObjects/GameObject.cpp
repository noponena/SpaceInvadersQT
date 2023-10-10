#include "GameObject.h"
#include "Utils/Utils.h"
#include <cmath>

namespace GameObjects {

GameObject::GameObject(const Position &position, float speed):
    m_targetPosition(0, 0), m_position(position), m_speed(speed),
    m_hasCollided(false), m_collidable(true)
{}

void GameObject::update(int deltaTime)
{
    this->execMovement(deltaTime);
    this->updateGraphicsItemPosition();
    this->updateBoundingBox();
}

void GameObject::moveLeft(int deltaTime)
{
    m_position.moveX(-std::round(m_speed * deltaTime));
}

void GameObject::moveRight(int deltaTime)
{
    m_position.moveX(std::round(m_speed * deltaTime));
}

void GameObject::moveDown(int deltaTime)
{
    m_position.moveY(std::round(m_speed * deltaTime));
}

void GameObject::moveUp(int deltaTime)
{
    m_position.moveY(-std::round(m_speed * deltaTime));
}

void GameObject::moveX(int amount)
{
    m_position.moveX(amount);
}

void GameObject::moveY(int amount)
{
    m_position.moveY(amount);
}

QGraphicsItem *GameObject::graphicsItem() const
{
    return m_graphicsItem;
}

const GameObject::BoundingBox &GameObject::boundingBox() const
{
    return m_boundingBox;
}

void GameObject::updateBoundingBox() {
    QSizeF size = m_graphicsItem->boundingRect().size();
    m_boundingBox.minX = m_position.getX() - size.width() / 2;
    m_boundingBox.minY = m_position.getY() - size.width() / 2;
    m_boundingBox.maxX = m_position.getX() + size.width() / 2;
    m_boundingBox.maxY = m_position.getY() + size.height() / 2;
}

void GameObject::move(int xRel, int yRel)
{
    this->moveX(xRel);
    this->moveY(yRel);
}

void GameObject::execMovement(int deltaTime) {
    if (m_xMovementFunc) {
        int newX = m_xMovementFunc(m_position.getX(), deltaTime);
        m_position.setX(newX);
    }
    if (m_yMovementFunc) {
        int newY = m_yMovementFunc(m_position.getY(), deltaTime);
        m_position.setY(newY);
    }
    updateGraphicsItemPosition();
}

void GameObject::updateGraphicsItemPosition()
{
    std::tuple<int, int> positionTuple = m_position.get();
    int x = std::get<0>(positionTuple);
    int y = std::get<1>(positionTuple);
    if (m_graphicsItem) {
        m_graphicsItem->setPos(x, y);
    }
}

void GameObject::setXMovementFunc(const MovementFunction::MovementFunc &newMovementFunc)
{
    m_xMovementFunc = newMovementFunc;
}

void GameObject::setYMovementFunc(const MovementFunction::MovementFunc &newYMovementFunc)
{
    m_yMovementFunc = newYMovementFunc;
}

bool GameObject::collidable() const
{
    return m_collidable;
}

bool GameObject::checkCollision(GameObject& other)
{
    if (!this->collidable() || !other.collidable())
    {
        return false;
    }
    BoundingBox localBox = this->boundingBox();
    BoundingBox otherBox = other.boundingBox();
    bool collision = !(localBox.minX > otherBox.maxX ||
                       localBox.maxX < otherBox.minX ||
                       localBox.minY > otherBox.maxY ||
                       localBox.maxY < otherBox.minY);

    if (collision) {
        this->collideWith(other);
        other.collideWith(*this);
    }

    return collision;
}

bool GameObject::isAtLimit() const
{
    return m_position.isBeyondLimit();
}

const Position &GameObject::position() const
{
    return m_position;
}

}
