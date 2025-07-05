#include "GameObject.h"
#include "Game/Audio/SoundManager.h"
#include "Graphics/Effects/EffectManager.h"
#include "Utils/BoundsChecker.h"
#include <QGraphicsScene>
#include <cmath>

namespace GameObjects {

std::uint32_t GameObject::counter = 0;

GameObject::GameObject(const Config::GameContext &ctx)
    : m_hasCollided(false), m_collidable(true), m_soundEnabled(true),
      m_magnetism({false, 0, 0}), m_id(counter++),
      m_hasDestructionEffect(false), m_destructionInitiated(false),
      m_gameContext(ctx) {

  m_state = State::Normal;
  m_objectTypes = {ObjectType::BASE};
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

  if (m_animationPlayer.isPlaying()) {
    auto frameUvs = m_animationPlayer.currentFrameUVs();
    setFrameUvsForCurrentState(frameUvs);
    m_animationPlayer.update(
        static_cast<int>(context.deltaTimeInSeconds * 1000));
  }
}

void GameObject::show() { m_visible = true; }

void GameObject::hide() { m_visible = false; }

void GameObject::playDestructionAnimation() {}

void GameObject::playDestructionEffects() {
  Graphics::Effects::EffectManager::instance().spawnDestructionEffect(
      QVector2D(getCenterPosition()), 1.0, 2000, 200);
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

void GameObject::setFrameUvsForCurrentState(
    const std::pair<QVector2D, QVector2D> frameUvs) {
  m_renderDataByState[m_state].uvMin = frameUvs.first;
  m_renderDataByState[m_state].uvMax = frameUvs.second;
}

void GameObject::executeDestructionProcedure() {
  m_state = State::OnDestruction;
  m_collidable = false;
  m_destructionInitiated = true;
  if (hasDestructionAnimation()) {
    m_animationPlayer.setAnimation(
        m_animationInfoByState[State::OnDestruction]);
    m_animationPlayer.play();
  }
  playDestructionSound();
  disableMovement();

  if (m_hasDestructionEffect)
    playDestructionEffects();

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

void GameObject::setState(State newState) { m_state = newState; }

State GameObject::state() const { return m_state; }

const RenderData GameObject::getRenderData() const {
  auto it = m_renderDataByState.find(m_state);

  if (it != m_renderDataByState.end())
    return it->second;

  return m_renderDataByState.at(State::Normal);
}

const RenderData &GameObject::getRenderData(State state) const {
  auto it = m_renderDataByState.find(state);
  if (it != m_renderDataByState.end())
    return it->second;
  return m_renderDataByState.at(State::Normal);
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

void GameObject::addRenderData(State state, const RenderData &data) {
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

bool GameObject::hasDestructionAnimation() const {
  return m_animationInfoByState.find(State::OnDestruction) !=
         m_animationInfoByState.end();
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
  m_destructionSoundInfo.enabled = newSoundEnabled;
}

void GameObject::addObjectType(const ObjectType objectType) {
  m_objectTypes.insert(objectType);
}

std::unordered_set<ObjectType> GameObject::objectTypes() const {
  return m_objectTypes;
}

} // namespace GameObjects
