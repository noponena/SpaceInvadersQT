#ifndef GAMEOBJECTS_COLLECTABLES_HEALTH_H
#define GAMEOBJECTS_COLLECTABLES_HEALTH_H

#include "GameObjects/Collectables/Collectable.h"

namespace GameObjects {
namespace Collectables {

class Health : public Collectable {
public:
  Health(const Config::GameContext &ctx);
  std::unique_ptr<GameObject> clone() const override;

protected:
  void initializeSounds() override;
  void initializeObjectType() override;

};

} // namespace Collectables
} // namespace GameObjects

#endif // GAMEOBJECTS_COLLECTABLES_HEALTH_H
