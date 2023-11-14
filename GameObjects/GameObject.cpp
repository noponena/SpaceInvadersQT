#include "GameObject.h"
#include "Game/Audio/SoundManager.h"
#include <cmath>

namespace GameObjects {

long long unsigned GameObject::counter = 0;

GameObject::GameObject(const Position &position)
    : m_position(position), m_hasCollided(false), m_collidable(true),
      m_destructionCompleted(false), m_soundEnabled(true),
      m_onHitPixmapResourcePath(""), m_id(counter++),
      m_destructionInitiated(false) {
  m_objectType = ObjectType::UNDEFINED;
}

void GameObject::initialize() {
  this->playSpawnSound();
  this->initializeGraphicsItem();
  this->updateGraphicsItemPosition();
  this->initializeDestructionAnimation();
  this->initializeDestructionEffects();
}

void GameObject::update(const UpdateContext &context) {
  if (this->isDead() && !m_destructionInitiated)
    this->initiateDestructionProcedure();
  this->applyMovementStrategy(context.deltaTimeInSeconds);
  this->updateGraphicsItemPosition();
}

void GameObject::applyMovementStrategy(float deltaTimeInSeconds) {
  std::pair<QPointF, QPointF> newPos = m_movementStrategy.move(
      m_position.pos, m_position.anchorPos, deltaTimeInSeconds);
  m_position.pos = newPos.first;
  m_position.anchorPos = newPos.second;
}

void GameObject::playSpawnSound() {
  Game::Audio::SoundManager::getInstance().playSoundEffect(m_spawnSoundInfo);
}

void GameObject::playDestructionSound() {
  Game::Audio::SoundManager::getInstance().playSoundEffect(
      m_destructionSoundInfo);
}

void GameObject::initiateDestructionProcedure() {
  m_collidable = false;
  m_destructionInitiated = true;
  this->playDestructionSound();
  this->disableMovement();
  this->playDestructionEffects();
  this->playDestructionAnimation();
}

QPointF GameObject::getPixmapScaledSize() const { return m_pixmapScale; }

QString GameObject::getPixmapResourcePath() const {
  return m_pixmapResourcePath;
}

QString GameObject::getOnHitPixmapResourcePath() const {
  if (m_onHitPixmapResourcePath.isEmpty())
    return this->getPixmapResourcePath();
  return m_onHitPixmapResourcePath;
}

QGraphicsItem *GameObject::getGraphicsItem() const { return m_graphicsItem; }

void GameObject::disableMovement() { m_movementStrategy.clear(); }

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

Game::Movement::MovementStrategy GameObject::movementStrategy() const {
  return m_movementStrategy;
}

void GameObject::setMovementStrategy(
    const Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = newMovementStrategy;
}

void GameObject::addMovementStrategy(
    Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = m_movementStrategy + newMovementStrategy;
}

long long unsigned GameObject::id() { return m_id; }

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

void GameObject::initializeGraphicsItem() {
  QPixmap pixmap = getPixmap();
  QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(pixmap);
  m_graphicsItem = pixmapItem;
}

bool GameObject::isAtLimit() const { return m_position.isBeyondAnyLimit(); }

Position GameObject::getPosition() const { return m_position; }

QPointF GameObject::getCenterPosition() const {
  QRectF sceneRect = this->getBoundingBox();
  return sceneRect.center();
}

void GameObject::setPosition(const Position &newPosition) {
  m_position = newPosition;
}

void GameObject::setPosition(const QPointF &newPosition) {
  m_position.setPos(newPosition);
}

void GameObject::setSoundEnabled(const bool newSoundEnabled) {
  m_spawnSoundInfo.enabled = newSoundEnabled;
}

ObjectType GameObject::objectType() const { return m_objectType; }

bool GameObject::isDestroyed() const { return m_destructionCompleted; }

} // namespace GameObjects
