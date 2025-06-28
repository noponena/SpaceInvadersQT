#include "GameObject.h"
#include "Game/Audio/SoundManager.h"
#include <QGraphicsScene>
#include <cmath>

namespace GameObjects {

std::uint64_t GameObject::counter = 0;

GameObject::GameObject(const Transform &transform,
                       const Config::GameContext ctx)
    : m_hasCollided(false), m_collidable(true), m_soundEnabled(true),
      m_magnetism({false, 0, 0}), m_id(counter++),
      m_destructionInitiated(false), m_gameContext(ctx) {

  m_renderState = RenderState::Normal;
  m_objectTypes = {ObjectType::BASE};
  setTransform(transform);
}

void GameObject::initialize() {
  initializeObjectType();
  initializeSounds();
  playSpawnSound();
  initializeDestructionAnimation();
  initializeDestructionEffects();
}

bool GameObject::shouldBeDeleted() {
  return Utils::BoundsChecker::isBeyondLimits(
      m_transform.position, m_gameContext.movementBounds, 25, 25, 50, 30);
}

void GameObject::update(const UpdateContext &context) {
  if (isDead() && !m_destructionInitiated)
    executeDestructionProcedure();
  applyMovementStrategy(context.deltaTimeInSeconds);
  if (m_destructionInitiated) {
    m_destructionAnimation.showNextFrame();
    m_destructionEffect.update(context.deltaTimeInSeconds);
  }
}

void GameObject::show() { m_visible = true; }

void GameObject::hide() { m_visible = false; }

void GameObject::playDestructionAnimation() {
  hide();
  //m_destructionAnimation.setPos(m_graphicsItem->pos());
  //getScene()->addItem(&m_destructionAnimation);
  m_destructionAnimation.start();
}

void GameObject::playDestructionEffects() {
  // qreal halfWidth = rect.width() / 2;
  // qreal halfHeight = rect.height() / 2;
  // QPointF p(m_position.x() + halfWidth, m_position.y() + halfHeight);
  m_destructionEffect.setPosition(m_transform.position.toPointF());
  // getScene()->addItem(&m_destructionEffect);
}

void GameObject::applyMovementStrategy(float deltaTimeInSeconds) {
  auto newPos = m_movementStrategy.move(
      m_transform.position, m_transform.anchorPosition, deltaTimeInSeconds);
  m_transform.position = QVector2D(newPos.first);
  m_transform.anchorPosition = QVector2D(newPos.second);
}

void GameObject::playSpawnSound() {
  Game::Audio::SoundManager::getInstance().playSoundEffect(m_spawnSoundInfo);
}

void GameObject::playDestructionSound() {
  Game::Audio::SoundManager::getInstance().playSoundEffect(
      m_destructionSoundInfo);
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

QRectF GameObject::getBoundingBox() const { return m_transform.colliderRect(); }

QString GameObject::hudPixmapPath() const {
  // return m_pixmapData.hudPixmapResourcePath;
  return "";
}

void GameObject::disableMovement() { m_movementStrategy.clear(); }

Game::Movement::MovementStrategy GameObject::movementStrategy() const {
  return m_movementStrategy;
}

void GameObject::setSpawnSoundInfo(
    const Game::Audio::SoundInfo &newSpawnSoundInfo) {
  m_spawnSoundInfo = newSpawnSoundInfo;
}

void GameObject::setDestructionSoundInfo(
    const Game::Audio::SoundInfo &newDestructionSoundInfo) {
  m_destructionSoundInfo = newDestructionSoundInfo;
}

void GameObject::setRenderState(RenderState newRenderState) {
  m_renderState = newRenderState;
}

RenderState GameObject::renderState() const { return m_renderState; }

const RenderData &GameObject::getRenderData() const {
  auto it = m_renderDataByState.find(m_renderState);
  if (it != m_renderDataByState.end())
    return it->second;
  return m_renderDataByState.at(RenderState::Normal);
}

const RenderData &GameObject::getRenderData(RenderState state) const {
  auto it = m_renderDataByState.find(state);
  if (it != m_renderDataByState.end())
    return it->second;
  return m_renderDataByState.at(RenderState::Normal);
}

RenderDataMap GameObject::renderDataByState() const {
  return m_renderDataByState;
}

void GameObject::setRenderDataByState(
    const RenderDataMap &newRenderDataByState) {
  m_renderDataByState = newRenderDataByState;
}

void GameObject::setTransform(const Transform &newTransform) {
  m_transform = newTransform;
}

void GameObject::setGameContext(const Config::GameContext &newGameContext) {
  m_gameContext = newGameContext;
}

Config::GameContext GameObject::gameContext() const { return m_gameContext; }

void GameObject::addRenderData(RenderState state, const RenderData &data) {
  m_renderDataByState[state] = data;
}

Transform GameObject::transform() const { return m_transform; }

void GameObject::setMovementStrategy(
    const Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = newMovementStrategy;
}

void GameObject::addMovementStrategy(
    Game::Movement::MovementStrategy &newMovementStrategy) {
  m_movementStrategy = m_movementStrategy + newMovementStrategy;
}

std::uint64_t GameObject::id() const { return m_id; }

const Magnetism &GameObject::magnetism() const { return m_magnetism; }

void GameObject::clampToXBounds() {
  if (Utils::BoundsChecker::isBeyondScreenRight(m_transform.position,
                                                m_gameContext.movementBounds))
    m_transform.position.setX(m_gameContext.movementBounds.right());
  else if (Utils::BoundsChecker::isBeyondScreenLeft(
               m_transform.position, m_gameContext.movementBounds))
    m_transform.position.setX(m_gameContext.movementBounds.left());
}

void GameObject::clampToYBounds() {
  if (Utils::BoundsChecker::isBeyondScreenTop(m_transform.position,
                                              m_gameContext.movementBounds))
    m_transform.position.setY(m_gameContext.movementBounds.top());
  else if (Utils::BoundsChecker::isBeyondScreenBottom(
               m_transform.position, m_gameContext.movementBounds))
    m_transform.position.setY(m_gameContext.movementBounds.bottom());
}

bool GameObject::isCollidable() const { return m_collidable; }

bool GameObject::isVisible() const { return m_visible; }

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
  if (!m_collidable || !other.m_collidable)
    return false;
  return getBoundingBox().intersects(other.getBoundingBox());
}

const QVector2D &GameObject::getPosition() const {
  return m_transform.position;
}

QPointF GameObject::getCenterPosition() const {
  return getBoundingBox().center();
}

void GameObject::moveAbsolute(const QVector2D &newPosition) {
  m_transform.position = newPosition;
}

void GameObject::moveRelative(const QVector2D &displacement) {
  m_transform.position += displacement;
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
