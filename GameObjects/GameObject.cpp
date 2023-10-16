#include "GameObject.h"
#include "Utils/Utils.h"
#include <cmath>

namespace GameObjects {

int GameObject::counter = 0;

GameObject::GameObject(const Position &position, float speed)
    : m_position(position), m_speed(speed), m_hasCollided(false),
      m_collidable(true), m_id(counter++), m_destructionInitiated(false) {}

void GameObject::initialize() {
  this->initializeGraphicsItem();
  this->updateGraphicsItemPosition();
  this->initializeDestructionAnimation();
  this->initializeDestructionEffects();
}

void GameObject::update(float deltaTimeInSeconds) {
  if (this->isDestroyed() && !m_destructionInitiated)
    this->initiateDestructionProcedure();
  this->applyMovementStrategy(deltaTimeInSeconds);
  this->updateGraphicsItemPosition();
}

void GameObject::moveHorizontal(float deltaTimeInSeconds) {
  this->doMoveX(m_currentSpeedX * deltaTimeInSeconds);
}

void GameObject::moveVertical(float deltaTimeInSeconds) {
  this->doMoveY(m_currentSpeedY * deltaTimeInSeconds);
}

void GameObject::accelerateLeft(float deltaTimeInSeconds) {
  m_currentSpeedX -= m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedX < -m_speed) {
    m_currentSpeedX = -m_speed;
  }
}

void GameObject::accelerateRight(float deltaTimeInSeconds) {
  m_currentSpeedX += m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedX > m_speed) {
    m_currentSpeedX = m_speed;
  }
}

void GameObject::decelerateX(float deltaTimeInSeconds) {
  if (m_currentSpeedX > 0) {
    m_currentSpeedX -= m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedX < 0) {
      m_currentSpeedX = 0;
    }
  } else if (m_currentSpeedX < 0) {
    m_currentSpeedX += m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedX > 0) {
      m_currentSpeedX = 0;
    }
  }
}

void GameObject::accelerateUp(float deltaTimeInSeconds) {
  m_currentSpeedY -= m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedY < -m_speed) {
    m_currentSpeedY = -m_speed;
  }
}

void GameObject::accelerateDown(float deltaTimeInSeconds) {
  m_currentSpeedY += m_acceleration * deltaTimeInSeconds;
  if (m_currentSpeedY > m_speed) {
    m_currentSpeedY = m_speed;
  }
}

void GameObject::decelerateY(float deltaTimeInSeconds) {
  if (m_currentSpeedY > 0) {
    m_currentSpeedY -= m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedY < 0) {
      m_currentSpeedY = 0;
    }
  } else if (m_currentSpeedY < 0) {
    m_currentSpeedY += m_acceleration * deltaTimeInSeconds;
    if (m_currentSpeedY > 0) {
      m_currentSpeedY = 0;
    }
  }
}

void GameObject::moveX(float amount) { this->doMoveX(amount); }

void GameObject::moveY(float amount) { this->doMoveY(amount); }

void GameObject::doMoveX(float amount) {
  float current = m_position.x();
  m_position.setX(current + amount);
  this->clampToXBounds();
}

void GameObject::doMoveY(float amount) {
  float current = m_position.y();
  m_position.setY(current + amount);
  this->clampToYBounds();
}

void GameObject::applyMovementStrategy(float deltaTimeInSeconds) {
  std::pair<QPointF, QPointF> newPos = m_movementStrategy.move(
      m_position.pos, m_position.anchorPos, deltaTimeInSeconds);
  m_position.pos = newPos.first;
  m_position.anchorPos = newPos.second;
}

void GameObject::initiateDestructionProcedure() {
  m_collidable = false;
  m_destructionInitiated = true;
  this->clearMovementStrategy();
  this->playDestructionEffects();
  this->playDestructionAnimation();
}

QGraphicsItem *GameObject::getGraphicsItem() const { return m_graphicsItem; }

void GameObject::clearMovementStrategy() { m_movementStrategy.clear(); }

QPixmap GameObject::loadPixmap(const QString &path) const {
  QPixmap pixmap = QPixmap(path);
  return pixmap;
}

QPixmap GameObject::getPixmap() const {
  QPixmap pixmap = this->loadPixmap(this->getPixmapResourcePath());
  return this->scalePixmap(pixmap);
}

QPixmap GameObject::getOnHitPixmap() const {
  QPixmap pixmap = this->loadPixmap(this->getOnHitPixmapResourcePath());
  return this->scalePixmap(pixmap);
}

QPixmap GameObject::scalePixmap(QPixmap &pixmap) const {
  QPointF size = this->getPixmapScaledSize();
  pixmap = pixmap.scaled(size.x(), size.y(), Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
  return pixmap;
}

void GameObject::setMovementStrategy(
    const Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = newMovementStrategy;
}

int GameObject::id() { return m_id; }

void GameObject::clampToXBounds() {
  if (m_position.isBeyondScreenRightLimit())
    m_position.goToRightLimit();
  else if (m_position.isBeyondScreenLeftLimit())
    m_position.goToLeftLimit();
}

void GameObject::clampToYBounds() {
  if (m_position.isBeyondScreenTopLimit())
    m_position.goToTopLimit();
  else if (m_position.isBeyondScreenBottomLimit())
    m_position.goToBottomLimit();
}

void GameObject::clampToXYBounds() {
  this->clampToXBounds();
  this->clampToYBounds();
}

bool GameObject::isCollidable() const { return m_collidable; }

void GameObject::collide(GameObject &other) {
  int local_id = this->id();
  int other_id = other.id();
  if (this->m_collisions.find(other_id) == this->m_collisions.end()) {
    this->collideWith(other);
    m_collisions.insert(other_id);
  }

  if (other.m_collisions.find(local_id) == other.m_collisions.end()) {
    other.collideWith(*this);
    other.m_collisions.insert(local_id);
  }
}

bool GameObject::isCollidingWith(const GameObject &other) const {
  if (!this->m_collidable || !other.m_collidable) {
    return false;
  }

  return this->getGraphicsItem()->collidesWithItem(other.getGraphicsItem());
}

bool GameObject::isAtLimit() const { return m_position.isBeyondAnyLimit(); }

Position &GameObject::getPosition() { return m_position; }

} // namespace GameObjects
