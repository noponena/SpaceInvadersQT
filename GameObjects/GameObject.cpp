#include "GameObject.h"
#include "Utils/Utils.h"
#include <cmath>

namespace GameObjects {

GameObject::GameObject(const Position &position, float speed):
    m_targetPosition(0, 0), m_position(position), m_speed(speed),
    m_hasCollided(false), m_collidable(true)
{}

void GameObject::moveLeft(int deltaTime)
{
    m_position.moveX(-std::round(m_speed * deltaTime));
    this->calculateBoundingBox();
}

void GameObject::moveRight(int deltaTime)
{
    m_position.moveX(std::round(m_speed * deltaTime));
    this->calculateBoundingBox();
}

void GameObject::moveDown(int deltaTime)
{
    m_position.moveY(std::round(m_speed * deltaTime));
    this->calculateBoundingBox();
}

void GameObject::moveUp(int deltaTime)
{
    m_position.moveY(-std::round(m_speed * deltaTime));
    this->calculateBoundingBox();
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

void GameObject::calculateBoundingBox() {
    QSizeF size = m_graphicsItem->boundingRect().size();
    m_boundingBox.minX = m_position.getX() - size.width() / 2;
    m_boundingBox.minY = m_position.getY() - size.width() / 2;;
    m_boundingBox.maxX = m_position.getX() + size.width() / 2;
    m_boundingBox.maxY = m_position.getY() + size.height() / 2;
}

bool GameObject::collidable() const
{
    return m_collidable;
}

bool GameObject::isCollision(GameObject& other)
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
//        qDebug() << "localBox.minX:" << localBox.minX << "localBox.minY:" << localBox.minY;
//        qDebug() << "localBox.maxX:" << localBox.maxX << "localBox.maxY:" << localBox.maxY;
//        qDebug() << "otherBox.minX:" << otherBox.minX << "otherBox.minY:" << otherBox.minY;
//        qDebug() << "otherBox.maxX:" << otherBox.maxX << "otherBox.maxY:" << otherBox.maxY;
        this->collideWith(other);
        other.collideWith(*this);
    }

    return collision;
}

bool GameObject::isAtLimit() const
{
    return m_position.isAtLimit();
}

const Position &GameObject::position() const
{
    return m_position;
}

}
