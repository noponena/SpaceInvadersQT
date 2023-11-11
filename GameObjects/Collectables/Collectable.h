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
    Collectable(const Position &position, float speed)
        : GameObject(position, speed)
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

  void update(UpdateContext context) override {
      m_lifetimeElapsedMs += context.deltaTimeInSeconds * 1000;
      m_lifeSpanExceeded = m_lifetimeElapsedMs >= m_timeToLiveMs;
      GameObject::update(context);
      QPointF currentPlayerPosition = context.playerShip.getCenterPosition();
      Position currentPosition = this->getPosition();
      QPointF direction = currentPlayerPosition - currentPosition.pos;
      Ships::magnetism magnetism = context.playerShip.magnetism();
      bool isWithinMagneticRange = QVector2D(direction).length() < magnetism.magneticRadius;

      // Separate the initial and magnetic movements
      switch (m_movementState) {
        case MovementState::Initial:
          if (!m_hasInitiatedMovement) {
            float angle = QRandomGenerator::global()->bounded(2.0 * M_PI);
            float initialSpeed = QRandomGenerator::global()->bounded(50, 200);
            m_initialVelocity.setX(initialSpeed * cos(angle));
            m_initialVelocity.setY(initialSpeed * sin(angle));
            m_hasInitiatedMovement = true;
          }

          // Deceleration for initial movement
          m_initialVelocity *= m_dampingFactor;

          if (QVector2D(m_initialVelocity).length() < 0.1) {
            m_movementState = MovementState::Stopped;  // Transition to stopped state
          } else {
            // Apply initial movement
            currentPosition.setX(currentPosition.x() + m_initialVelocity.x() * context.deltaTimeInSeconds);
            currentPosition.setY(currentPosition.y() + m_initialVelocity.y() * context.deltaTimeInSeconds);
            this->setPosition(currentPosition);
          }
          break;

        case MovementState::Magnetic:
          // Magnetic movement logic
          if (!isWithinMagneticRange) {
              m_movementState = MovementState::Stopped;
              break;
          }

          direction = QVector2D(direction).normalized().toPointF() * magnetism.magneticStrength;

          m_magneticVelocity += direction;
          m_magneticVelocity *= m_dampingFactor;
          currentPosition.setX(currentPosition.x() + m_magneticVelocity.x() * context.deltaTimeInSeconds);
          currentPosition.setY(currentPosition.y() + m_magneticVelocity.y() * context.deltaTimeInSeconds);
          this->setPosition(currentPosition);
          break;

        case MovementState::Stopped:
          if (magnetism.isMagnetic) {
            if (isWithinMagneticRange) {
              m_movementState = MovementState::Magnetic;
            }
          }
          break;
      }
    }

  // GameObject interface
public:
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override {
      Q_UNUSED(playerShip);
      m_collected = true;
  };
private:
  MovementState m_movementState = MovementState::Initial;
  QPointF m_initialVelocity;
  QPointF m_magneticVelocity;

protected:
  bool isDead() override { return m_collected; }
};

} // namespace Collectables
} // namespace GameObjects

#endif // COLLECTABLE_H
