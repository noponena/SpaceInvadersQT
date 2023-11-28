#include "MagneticGameObject.h"
#include <QVector2D>

namespace GameObjects {

MagneticGameObject::MagneticGameObject(const Position &position)
    : GameObject(position), m_movementState(MovementState::Initial),
      m_magneticVelocity(QPointF(0, 0)) {}

void MagneticGameObject::update(const UpdateContext &context) {
  GameObject::update(context);
  updateMovement(context);
}

void MagneticGameObject::updateMovement(const UpdateContext &context) {
  std::pair<QPointF, Magnetism> data = getMagneticObjectData(context);
  QPointF magneticObjectPosition = data.first;
  Magnetism magnetism = data.second;
  Position currentPosition = getPosition();
  QPointF direction = magneticObjectPosition - currentPosition.pos;
  bool isWithinMagneticRange =
      QVector2D(direction).length() < magnetism.magneticRadius;

  switch (m_movementState) {
  case MovementState::Initial:
    handleInitialMovement(context.deltaTimeInSeconds);
    break;
  case MovementState::Magnetic:
    handleMagneticMovement(direction, magnetism, context.deltaTimeInSeconds,
                           isWithinMagneticRange);
    break;
  case MovementState::Stopped:
    handleStoppedState(magnetism, isWithinMagneticRange);
    break;
  }
}

void MagneticGameObject::applyMovement(float deltaTimeInSeconds,
                                       const QPointF &velocity) {
  Position currentPosition = getPosition();
  currentPosition.setX(currentPosition.x() + velocity.x() * deltaTimeInSeconds);
  currentPosition.setY(currentPosition.y() + velocity.y() * deltaTimeInSeconds);
  setPosition(currentPosition);
}

void MagneticGameObject::handleMagneticMovement(const QPointF &direction,
                                                Magnetism &magnetism,
                                                float deltaTimeInSeconds,
                                                bool isWithinRange) {
  if (!isWithinRange) {
    m_movementState = MovementState::Stopped;
    return;
  }

  QPointF magneticForce =
      QVector2D(direction).normalized().toPointF() * magnetism.magneticStrength;
  m_magneticVelocity += magneticForce;
  m_magneticVelocity *= m_dampingFactor;
  applyMovement(deltaTimeInSeconds, m_magneticVelocity);
}

void MagneticGameObject::handleStoppedState(Magnetism &magnetism,
                                            bool isWithinRange) {
  if (magnetism.isMagnetic && isWithinRange) {
    m_movementState = MovementState::Magnetic;
  }
}

void MagneticGameObject::handleInitialMovement(float deltaTimeInSeconds) {
  Q_UNUSED(deltaTimeInSeconds);
  m_movementState = MovementState::Stopped;
}

} // namespace GameObjects
