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
    m_boundingBox.minX = m_position.x - m_boundingBoxWidthHalf;
    m_boundingBox.minY = m_position.y - m_boundingBoxHeightHalf;
    m_boundingBox.maxX = m_position.x + m_boundingBoxWidthHalf;
    m_boundingBox.maxY = m_position.y + m_boundingBoxHeightHalf;
}

void GameObject::clearMovementStrategy()
{
    m_movementStrategy.clear();
}

void GameObject::move(int xRel, int yRel)
{
    this->moveX(xRel);
    this->moveY(yRel);
}

void GameObject::setMovementStrategy(const Game::MovementStrategies::MovementStrategy &newMovementStrategy)
{
    m_movementStrategy = newMovementStrategy;
}

void GameObject::initBoundingBox()
{
    QSizeF size = m_graphicsItem->boundingRect().size();
    m_boundingBoxWidth = size.width();
    m_boundingBoxHeight = size.height();
    m_boundingBoxWidthHalf = m_boundingBoxWidth / 2;
    m_boundingBoxHeightHalf = m_boundingBoxHeight / 2;
    this->updateBoundingBox();
}

void GameObject::execMovement(int deltaTime) {
    std::tuple<int, int> newPos = m_movementStrategy.move(m_position.get(), deltaTime);
    m_position.x = std::get<0>(newPos);
    m_position.y = std::get<1>(newPos);
}

void GameObject::updateGraphicsItemPosition()
{
    if (m_graphicsItem) {
        m_graphicsItem->setPos(m_position.x, m_position.y);
    }
}

bool GameObject::collidable() const
{
    return m_collidable;
}

void GameObject::doCollide(GameObject& other)
{
    if (!this->collidable() || !other.collidable())
    {
        return;
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
}

bool GameObject::isCollision(const GameObject &other) const
{
    if (!this->collidable() || !other.collidable())
    {
        return false;
    }

    BoundingBox localBox = this->boundingBox();
    BoundingBox otherBox = other.boundingBox();
    return !(localBox.minX > otherBox.maxX ||
                       localBox.maxX < otherBox.minX ||
                       localBox.minY > otherBox.maxY ||
                       localBox.maxY < otherBox.minY);
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
