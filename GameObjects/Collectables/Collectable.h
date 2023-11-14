#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "GameObjects/Ships/Ship.h"
#include <QVector2D>
#include <QRandomGenerator>


namespace GameObjects {
namespace Collectables {

enum class MovementState {
  Initial,
  Magnetic,
  Stopped
};

class Collectable : public GameObject {
public:
    Collectable(const Position &position)
        : GameObject(position)
    {
        m_magneticVelocity = QPointF(0, 0);
    }
    ~Collectable() {};

  bool m_collected = false;
  bool m_hasInitiatedMovement = false;
  bool m_stopped = false;
  float m_dampingFactor = 0.9f;
  float m_timeToLiveMs = 30000;
  float m_lifetimeElapsedMs = 0;
  bool m_lifeSpanExceeded = false;
  float m_blinkAccumulator = 0.0f;

  void update(const UpdateContext &context) override {
        if (context.playerShip) {
            updateLifetime(context.deltaTimeInSeconds);
            handleBlinking(context.deltaTimeInSeconds);
            GameObject::update(context);
            updateMovement(context);
        }
  }

  bool shouldBeDeleted() override {
       return m_collected || m_lifeSpanExceeded || m_position.isBeyondAnyLimit();
  }
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override {
      Q_UNUSED(playerShip);
      m_collected = true;
  };
private:
  MovementState m_movementState = MovementState::Initial;
  QPointF m_initialVelocity;
  QPointF m_magneticVelocity;
private:
  inline void updateLifetime(float deltaTimeInSeconds) {
      m_lifetimeElapsedMs += deltaTimeInSeconds * 1000;
      m_lifeSpanExceeded = m_lifetimeElapsedMs >= m_timeToLiveMs;
  }

  inline void handleBlinking(float deltaTimeInSeconds) {
      float remainingLifePercent = 1.0f - (m_lifetimeElapsedMs / m_timeToLiveMs);
      const float startBlinkingAt = 0.2f;

      if (remainingLifePercent <= startBlinkingAt) {
            m_blinkAccumulator += deltaTimeInSeconds;
            bool visible = shouldBlinkVisible(remainingLifePercent, m_blinkAccumulator);
            m_graphicsItem->setVisible(visible);
      }
  }

  inline bool shouldBlinkVisible(float remainingLifePercent, float accumulator) {
      const float startFrequency = 1.0f;
      const float maxFrequency = 5.0f;
      float frequencyRange = maxFrequency - startFrequency;
      float adjustedLifePercent = (0.2f - remainingLifePercent) / 0.2f;
      float frequency = startFrequency + (frequencyRange * adjustedLifePercent);
      return static_cast<int>(accumulator * frequency) % 2 == 0;
  }

  inline void updateMovement(const UpdateContext &context) {
      QPointF currentPlayerPosition = context.playerShip->getCenterPosition();
      Position currentPosition = this->getPosition();
      QPointF direction = currentPlayerPosition - currentPosition.pos;
      Ships::magnetism magnetism = context.playerShip->magnetism();
      bool isWithinMagneticRange = QVector2D(direction).length() < magnetism.magneticRadius;

      switch (m_movementState) {
      case MovementState::Initial:
            handleInitialMovement(context.deltaTimeInSeconds);
            break;
      case MovementState::Magnetic:
            handleMagneticMovement(direction, magnetism, context.deltaTimeInSeconds, isWithinMagneticRange);
            break;
      case MovementState::Stopped:
            handleStoppedState(magnetism, isWithinMagneticRange);
            break;
      }
  }

  inline void handleInitialMovement(float deltaTimeInSeconds) {
      if (!m_hasInitiatedMovement) {
            initiateMovement();
      }

      m_initialVelocity *= m_dampingFactor;

      if (QVector2D(m_initialVelocity).length() < 0.1) {
            m_movementState = MovementState::Stopped;  // Transition to stopped state
      } else {
            applyMovement(deltaTimeInSeconds, m_initialVelocity);
      }
  }

  inline void initiateMovement() {
      float angle = QRandomGenerator::global()->bounded(2.0 * M_PI);
      float initialSpeed = QRandomGenerator::global()->bounded(50, 200);
      m_initialVelocity.setX(initialSpeed * cos(angle));
      m_initialVelocity.setY(initialSpeed * sin(angle));
      m_hasInitiatedMovement = true;
  }

  inline void applyMovement(float deltaTimeInSeconds, const QPointF &velocity) {
      Position currentPosition = this->getPosition();
      currentPosition.setX(currentPosition.x() + velocity.x() * deltaTimeInSeconds);
      currentPosition.setY(currentPosition.y() + velocity.y() * deltaTimeInSeconds);
      this->setPosition(currentPosition);
  }

  inline void handleMagneticMovement(const QPointF &direction, Ships::magnetism &magnetism, float deltaTimeInSeconds, bool isWithinRange) {
      if (!isWithinRange) {
            m_movementState = MovementState::Stopped;
            return;
      }

      QPointF magneticForce = QVector2D(direction).normalized().toPointF() * magnetism.magneticStrength;
      m_magneticVelocity += magneticForce;
      m_magneticVelocity *= m_dampingFactor;
      applyMovement(deltaTimeInSeconds, m_magneticVelocity);
  }

  inline void handleStoppedState(Ships::magnetism &magnetism, bool isWithinRange) {
      if (magnetism.isMagnetic && isWithinRange) {
            m_movementState = MovementState::Magnetic;
      }
  }

protected:
  bool isDead() override { return m_collected; }
};

} // namespace Collectables
} // namespace GameObjects

#endif // COLLECTABLE_H
