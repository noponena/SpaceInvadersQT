#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "GameObjects/GameObject.h"
#include <QRandomGenerator>

namespace GameObjects {
namespace Collectables {

class Collectable : public GameObject {
public:
    Collectable(const Position &position, float speed)
        : GameObject(position, speed) {}
  bool m_isMagnetic = false;
  bool m_collected = false;
  bool m_hasInitiatedMovement = false;
  bool m_stopped = false;
  float m_magneticRadius = 100.0f;
  float m_magneticStrength = 1.0f;
  QPointF m_velocity;

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

  void update(float deltaTime) override {
    GameObject::update(deltaTime);
    // if (m_isMagnetic) {
      // Pseudocode to move towards player
      // Vector2D direction = player.position - this->position;
      // if (direction.length() < m_magneticRadius) {
      //    this->position += direction.normalized() * m_magneticStrength * deltaTime;
      // }
    if (!m_hasInitiatedMovement) {
      // Set the random seed for random number generation, ideally somewhere at the start of the program
      srand(static_cast<unsigned int>(time(nullptr)));

      float angle = QRandomGenerator::global()->bounded(2.0 * M_PI);
      float initialSpeed = 20;
      m_velocity.setX(initialSpeed * cos(angle));
      m_velocity.setY(initialSpeed * sin(angle));
      m_hasInitiatedMovement = true;
    }

    if (!m_stopped) {
        // Apply deceleration as a factor of the current speed, ensuring it always decreases
        float decelerationFactor = 0.9;
        m_velocity.setX(m_velocity.x() * decelerationFactor);
        m_velocity.setY(m_velocity.y() * decelerationFactor);

        //qDebug() << "manhattanLength:" << m_velocity.manhattanLength();

        if (m_velocity.manhattanLength() < 0.1) {
          m_stopped = true;
        }

        // Update the position based on the current velocity
        Position currentPosition = this->getPosition();
        currentPosition.setX(currentPosition.x() + m_velocity.x() * deltaTime * 10);
        currentPosition.setY(currentPosition.y() + m_velocity.y() * deltaTime * 10);
        this->setPosition(currentPosition);
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
};

} // namespace Collectables
} // namespace GameObjects

#endif // COLLECTABLE_H
