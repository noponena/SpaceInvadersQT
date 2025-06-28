#include "Collectable.h"
#include "Utils/Math/MathConstants.h"

namespace GameObjects {
namespace Collectables {

Collectable::Collectable(const Config::GameContext &ctx)
    : AttractableGameObject(ctx) {
  m_magneticTargets = {ObjectType::PLAYER_SHIP};
  m_attractionEnabled = false;
}

void GameObjects::Collectables::Collectable::update(
    const UpdateContext &context) {
  updateLifetime(context.deltaTimeInSeconds);
  handleBlinking(context.deltaTimeInSeconds);
  handleInitialMovement(context.deltaTimeInSeconds);
  if (context.playerShip) {
    AttractableGameObject::update(context);
  }
}

bool Collectable::shouldBeDeleted() {
  return GameObject::shouldBeDeleted() || m_collected || m_lifeSpanExceeded;
}

void Collectable::collideWithPlayerShip(Ships::PlayerShip &playerShip) {
  Q_UNUSED(playerShip);
  m_collected = true;
}

void Collectable::updateLifetime(float deltaTimeInSeconds) {
  m_lifetimeElapsedMs += deltaTimeInSeconds * 1000;
  m_lifeSpanExceeded = m_lifetimeElapsedMs >= m_timeToLiveMs;
}

void Collectable::handleBlinking(float deltaTimeInSeconds) {
  float remainingLifePercent = 1.0f - (m_lifetimeElapsedMs / m_timeToLiveMs);
  const float startBlinkingAt = 0.2f;

  if (remainingLifePercent <= startBlinkingAt) {
    m_blinkAccumulator += deltaTimeInSeconds;
    bool visible = shouldBlinkVisible(remainingLifePercent, m_blinkAccumulator);
    if (visible) {
      show();
    } else {
      hide();
    }
  }
}

bool Collectable::shouldBlinkVisible(float remainingLifePercent,
                                     float accumulator) {
  const float startFrequency = 1.0f;
  const float maxFrequency = 5.0f;
  float frequencyRange = maxFrequency - startFrequency;
  float adjustedLifePercent = (0.2f - remainingLifePercent) / 0.2f;
  float frequency = startFrequency + (frequencyRange * adjustedLifePercent);
  return static_cast<int>(accumulator * frequency) % 2 == 0;
}

void Collectable::handleInitialMovement(float deltaTimeInSeconds) {
  if (!m_attractionEnabled) {
    if (!m_hasInitiatedMovement) {
      initiateMovement();
    }

    m_initialVelocity *= m_dampingFactor;

    if (QVector2D(m_initialVelocity).length() < 0.1) {
      m_attractionEnabled = true;
    } else {
      applyMovement(deltaTimeInSeconds, m_initialVelocity);
    }
  }
}

void Collectable::initiateMovement() {
  float angle = QRandomGenerator::global()->bounded(2.0 * PI);
  float initialSpeed = QRandomGenerator::global()->bounded(50, 200);
  m_initialVelocity.setX(initialSpeed * cos(angle));
  m_initialVelocity.setY(initialSpeed * sin(angle));
  m_hasInitiatedMovement = true;
}

void Collectable::initializeObjectType() {
  m_objectTypes.insert(ObjectType::COLLECTABLE);
}

} // namespace Collectables
} // namespace GameObjects
