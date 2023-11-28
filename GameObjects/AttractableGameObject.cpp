#include "AttractableGameObject.h"
#include <QVector2D>

namespace GameObjects {

AttractableGameObject::AttractableGameObject(const Position &position)
    : GameObject(position), m_attractionEnabled(true),
      m_magneticVelocity(QPointF(0, 0)) {}

void AttractableGameObject::update(const UpdateContext &context) {
  GameObject::update(context);
  updateMovement(context);
}

void AttractableGameObject::updateMovement(const UpdateContext &context) {
  if (!m_attractionEnabled)
    return;

  std::vector<std::pair<QPointF, Magnetism>> data =
      getMagneticObjectData(context);
  QPointF totalMagneticForce(0, 0);
  bool isWithinAnyMagneticRange = false;

  for (const auto &pair : data) {
    QPointF magneticObjectPosition = pair.first;
    Magnetism magnetism = pair.second;
    QPointF currentPosition = getCenterPosition();
    QPointF direction = magneticObjectPosition - currentPosition;
    bool isWithinMagneticRange =
        QVector2D(direction).length() < magnetism.magneticRadius;

    if (isWithinMagneticRange) {
      isWithinAnyMagneticRange = true;
      QPointF magneticForce = QVector2D(direction).normalized().toPointF() *
                              magnetism.magneticStrength;
      totalMagneticForce += magneticForce;
    }
  }

  if (isWithinAnyMagneticRange)
    handleMagneticMovement(totalMagneticForce, context.deltaTimeInSeconds);
}

void AttractableGameObject::applyMovement(float deltaTimeInSeconds,
                                          const QPointF &velocity) {
  Position currentPosition = getPosition();
  currentPosition.setX(currentPosition.x() + velocity.x() * deltaTimeInSeconds);
  currentPosition.setY(currentPosition.y() + velocity.y() * deltaTimeInSeconds);
  setPosition(currentPosition);
}

void AttractableGameObject::handleMagneticMovement(
    const QPointF &totalMagneticForce, float deltaTimeInSeconds) {
  QPointF normalizedForce =
      QVector2D(totalMagneticForce).normalized().toPointF();
  m_magneticVelocity += normalizedForce * 100;
  m_magneticVelocity *= m_dampingFactor;
  applyMovement(deltaTimeInSeconds, m_magneticVelocity);
}

const std::vector<std::pair<QPointF, Magnetism>>
AttractableGameObject::getMagneticObjectData(const UpdateContext &context) {
  std::vector<std::pair<QPointF, Magnetism>> magneticData;
  magneticData.reserve(context.magneticGameObjects.size());

  for (const auto &objectPair : context.magneticGameObjects) {
    auto &object = objectPair.second;
    if (object->magnetism().enabled) {
      for (const auto &targetType : m_magneticTargets) {
        if (object->objectTypes().find(targetType) !=
            object->objectTypes().end()) {
          if (m_objectTypes.find(ObjectType::COLLECTABLE) !=
              m_objectTypes.end()) {
            magneticData.push_back(
                {object->getCenterPosition(), object->magnetism()});
          } else {
            magneticData.push_back(
                {object->getCenterPosition() /*+ QPointF(-25, -25)*/,
                 object->magnetism()});
          }
          break;
        }
      }
    }
  }

  return magneticData;
}

} // namespace GameObjects
