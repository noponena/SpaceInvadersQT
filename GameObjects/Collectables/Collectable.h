#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "GameObjects/GameObject.h"

namespace GameObjects {
namespace Collectables {

class Collectable : public GameObjects::GameObject {
public:
  bool m_isMagnetic = false;
  float m_magneticRadius = 100.0f;
  float m_magneticStrength = 1.0f;

  void activateMagneticEffect() {
    m_isMagnetic = true;
  }

  void deactivateMagneticEffect() {
    m_isMagnetic = false;
  }

  void update(float deltaTime) override {
    GameObject::update(deltaTime);
    if (m_isMagnetic) {
      // Pseudocode to move towards player
      // Vector2D direction = player.position - this->position;
      // if (direction.length() < m_magneticRadius) {
      //    this->position += direction.normalized() * m_magneticStrength * deltaTime;
      // }
    }
  }
};

} // namespace Collectables
} // namespace GameObjects

#endif // COLLECTABLE_H
