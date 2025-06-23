#include "GameObject.h"
#include "Game/Audio/SoundManager.h"
#include "Graphics/PixmapLibrary.h"
#include <QGraphicsScene>
#include <cmath>

namespace GameObjects {

std::uint64_t GameObject::counter = 0;

GameObject::GameObject(const Position &position)
    : m_position(position), m_hasCollided(false), m_collidable(true),
      m_soundEnabled(true), m_magnetism({false, 0, 0}), m_id(counter++),
      m_destructionInitiated(false) {
  m_objectTypes = {ObjectType::BASE};
  m_pixmapData = PixmapData{QPointF(30, 30), ":/Images/placeholder.png", ""};
}

void GameObject::initialize() {
  initializeObjectType();
  initializeSounds();
  playSpawnSound();
  m_graphicsItem = std::make_unique<QGraphicsPixmapItem>(getPixmap());
  initializeDestructionAnimation();
  initializeDestructionEffects();
}

bool GameObject::shouldBeDeleted() {
  return m_position.isBeyondLimits(25, 25, 50, 30);
}

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

QString GameObject::getHudPixmapPath() const {
  return m_pixmapData.hudPixmapResourcePath;
}

void GameObject::disableMovement() { m_movementStrategy.clear(); }

QPixmap GameObject::getPixmap() const {
  return Graphics::PixmapLibrary::getPixmap(
      m_pixmapData.pixmapResourcePath, m_pixmapData.pixmapScale.x(),
      m_pixmapData.pixmapScale.y(), m_pixmapData.keepAspectRatio);
}

QPixmap GameObject::getOnHitPixmap() const {
  QString path = m_pixmapData.onHitPixmapResourcePath;
  if (path.isEmpty())
    return getPixmap();
  return Graphics::PixmapLibrary::getPixmap(path, m_pixmapData.pixmapScale.x(),
                                            m_pixmapData.pixmapScale.y(),
                                            m_pixmapData.keepAspectRatio);
}

Game::Movement::MovementStrategy GameObject::movementStrategy() const {
  return m_movementStrategy;
}

void GameObject::setPixmapData(const PixmapData &newPixmapData) {
  m_pixmapData = newPixmapData;
}

void GameObject::setSpawnSoundInfo(
    const Game::Audio::SoundInfo &newSpawnSoundInfo) {
  m_spawnSoundInfo = newSpawnSoundInfo;
}

void GameObject::setDestructionSoundInfo(
    const Game::Audio::SoundInfo &newDestructionSoundInfo) {
  m_destructionSoundInfo = newDestructionSoundInfo;
}

void GameObject::setMovementStrategy(
    const Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = newMovementStrategy;
}

void GameObject::addMovementStrategy(
    Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = m_movementStrategy + newMovementStrategy;
}

std::uint64_t
GameObject::id ()
{
  return m_id;
}

const Magnetism &GameObject::magnetism() const { return m_magnetism; }

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
  ;
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

void GameObject::addObjectType(const ObjectType objectType) {
  m_objectTypes.insert(objectType);
}

std::unordered_set<ObjectType> GameObject::objectTypes() const {
  return m_objectTypes;
}

} // namespace GameObjects
