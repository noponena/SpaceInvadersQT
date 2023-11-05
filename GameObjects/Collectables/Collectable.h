#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "GameObjects/GameObject.h"
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
  bool m_isMagnetic = false;
  bool m_collected = false;
  bool m_hasInitiatedMovement = false;
  bool m_stopped = false;
  float m_magneticRadius = 200.0f;
  float m_magneticStrength = 100.0f;

  void activateMagneticEffect() {
    m_isMagnetic = true;
  }

  void deactivateMagneticEffect() {
    m_isMagnetic = false;
  }

  void reset() {
      m_hasInitiatedMovement = false;
      m_stopped = false;
      m_collected = false;
      m_hasCollided = false;
      m_collisions.clear();
      m_collidable = true;
  }

  void update(UpdateContext context) override {
      GameObject::update(context);
      QPointF currentPlayerPosition = context.playerPosition;
      Position currentPosition = this->getPosition();
      QPointF direction = currentPlayerPosition - currentPosition.pos;
      bool isWithinMagneticRange = QVector2D(direction).length() < m_magneticRadius;
      float dampingFactor = 0.9f;

      // Separate the initial and magnetic movements
      switch (m_movementState) {
        case MovementState::Initial:
          if (!m_hasInitiatedMovement) {
            float angle = QRandomGenerator::global()->bounded(2.0 * M_PI);
            float initialSpeed = 200;
            m_initialVelocity.setX(initialSpeed * cos(angle));
            m_initialVelocity.setY(initialSpeed * sin(angle));
            m_hasInitiatedMovement = true;
          }

          // Deceleration for initial movement
          m_initialVelocity *= dampingFactor;

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

          direction = QVector2D(direction).normalized().toPointF() * m_magneticStrength;

          m_magneticVelocity += direction;
          m_magneticVelocity *= dampingFactor;
          currentPosition.setX(currentPosition.x() + m_magneticVelocity.x() * context.deltaTimeInSeconds);
          currentPosition.setY(currentPosition.y() + m_magneticVelocity.y() * context.deltaTimeInSeconds);
          this->setPosition(currentPosition);
          break;

        case MovementState::Stopped:
          if (m_isMagnetic) {
            if (isWithinMagneticRange) {
              m_movementState = MovementState::Magnetic;
            }
          }
          break;
      }
    }

  // GameObject interface
public:
  bool shouldBeDeleted() override;

protected:
  QPointF getPixmapScaledSize() const override;
  QString getPixmapResourcePath() const override;

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
};

} // namespace Collectables
} // namespace GameObjects

#endif // COLLECTABLE_H
