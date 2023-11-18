#include "GameObject.h"
#include "Game/Audio/SoundManager.h"
#include "Graphics/PixmapLibrary.h"
#include <QGraphicsScene>
#include <cmath>

namespace GameObjects {

unsigned long long int GameObject::counter = 0;

GameObject::GameObject(const Position &position)
    : m_position(position), m_hasCollided(false), m_collidable(true),
      m_soundEnabled(true), m_onHitPixmapResourcePath(""), m_id(counter++),
      m_destructionInitiated(false) {
  m_objectType = ObjectType::UNDEFINED;
}

void GameObject::initialize() {
  initializeObjectType();
  initializeSounds();
  playSpawnSound();
  initializeGraphics();
  m_graphicsItem = std::make_unique<QGraphicsPixmapItem>(getPixmap());
  initializeDestructionAnimation();
  initializeDestructionEffects();
}

bool GameObject::shouldBeDeleted() { return m_position.isBeyondAnyLimit(50); }

void GameObject::update(const UpdateContext &context) {
  if (isDead() && !m_destructionInitiated)
    executeDestructionProcedure();
  applyMovementStrategy(context.deltaTimeInSeconds);
  updateGraphicsItemPosition();
  if (m_destructionInitiated)
    m_destructionAnimation.showNextFrame();
}

void GameObject::show() { m_graphicsItem->show(); }

void GameObject::hide() { m_graphicsItem->hide(); }

void GameObject::playDestructionAnimation() {
  hide();
  m_destructionAnimation.setPos(m_graphicsItem->pos());
  getScene()->addItem(&m_destructionAnimation);
  m_destructionAnimation.start();
}

void GameObject::playDestructionEffects() {
  QRectF rect = m_graphicsItem->boundingRect();
  qreal halfWidth = rect.width() / 2;
  qreal halfHeight = rect.height() / 2;
  QPointF p(m_position.x() + halfWidth, m_position.y() + halfHeight);
  m_destructionEffect.setPosition(p);
  getScene()->addItem(&m_destructionEffect);
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

void GameObject::updateGraphicsItemPosition() {
  if (m_graphicsItem) {
    m_graphicsItem->setPos(m_position.pos);
  }
}

void GameObject::executeDestructionProcedure() {
  m_collidable = false;
  m_destructionInitiated = true;

  playDestructionSound();
  disableMovement();

  if (m_destructionEffect)
    playDestructionEffects();
  if (m_destructionAnimation)
    playDestructionAnimation();

  emit objectDestroyed();
}

QGraphicsPixmapItem *GameObject::getGraphicsItem() const {
  return m_graphicsItem.get();
}

QRectF GameObject::getBoundingBox() const {
  QRectF localRect = m_graphicsItem->boundingRect();
  QRectF sceneRect = m_graphicsItem->mapToScene(localRect).boundingRect();
  return sceneRect;
}

void GameObject::disableMovement() { m_movementStrategy.clear(); }

QPixmap GameObject::getPixmap() const {
  return Graphics::PixmapLibrary::getPixmap(
      m_pixmapResourcePath, m_pixmapScale.x(), m_pixmapScale.y());
}

QPixmap GameObject::getOnHitPixmap() const {
  QString path = m_onHitPixmapResourcePath;
  if (path.isEmpty())
    path = m_pixmapResourcePath;
  return Graphics::PixmapLibrary::getPixmap(path, m_pixmapScale.x(),
                                            m_pixmapScale.y());
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

bool GameObject::isCollidable() const { return m_collidable; }

void GameObject::collide(GameObject &other) {
  int local_id = id();
  int other_id = other.id();
  if (m_collisions.find(other_id) == m_collisions.end()) {
    collideWith(other);
    m_collisions.insert(other_id);
  }

  if (other.m_collisions.find(local_id) == other.m_collisions.end()) {
    other.collideWith(*this);
    other.m_collisions.insert(local_id);
  }
}

bool GameObject::isCollidingWith(const GameObject &other) const {
  if (!m_collidable || !other.m_collidable) {
    return false;
  }

  return getGraphicsItem()->collidesWithItem(other.getGraphicsItem());
}

Position GameObject::getPosition() const { return m_position; }

QPointF GameObject::getCenterPosition() const {
  QRectF sceneRect = getBoundingBox();
  return sceneRect.center();
}

QGraphicsScene *GameObject::getScene() const { return m_graphicsItem->scene(); }

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

} // namespace GameObjects
