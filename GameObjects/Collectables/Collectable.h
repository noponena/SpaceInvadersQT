#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "GameObjects/AttractableGameObject.h"
#include "GameObjects/Ships/PlayerShip.h"
#include <QRandomGenerator>
#include <QVector2D>

namespace GameObjects {
namespace Collectables {

enum class MovementState { Initial, Magnetic, Stopped };

class Collectable : public AttractableGameObject {
public:
  Collectable(const Config::GameContext &ctx);
  ~Collectable() {};

  bool m_collected = false;
  bool m_hasInitiatedMovement = false;
  float m_timeToLiveMs = 30000;
  float m_lifetimeElapsedMs = 0;
  bool m_lifeSpanExceeded = false;
  float m_blinkAccumulator = 0.0f;

  void update(const UpdateContext &context) override;

  bool shouldBeDeleted() override;
  void collideWithPlayerShip(Ships::PlayerShip &playerShip) override;

private:
  QPointF m_initialVelocity;

private:
  inline void updateLifetime(float deltaTimeInSeconds);
  inline void handleBlinking(float deltaTimeInSeconds);
  inline bool shouldBlinkVisible(float remainingLifePercent, float accumulator);

  inline void handleInitialMovement(float deltaTimeInSeconds);
  inline void initiateMovement();

protected:
  void initializeObjectType() override;
  bool isDead() override { return m_collected; }

  // GameObject interface
public:
  void collideWith(GameObject &other) override {
    other.collideWithCollectable(*this);
  }
};

} // namespace Collectables
} // namespace GameObjects

#endif // COLLECTABLE_H
