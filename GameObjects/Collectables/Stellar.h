#ifndef STELLAR_H
#define STELLAR_H

#include "Collectable.h"

namespace GameObjects {
namespace Collectables {

class Stellar : public Collectable {
public:
  Stellar(const Config::GameContext &ctx);
  std::unique_ptr<GameObject> clone() const override;

protected:
  void initializeSounds() override;
  void initializeObjectType() override;
};
} // namespace Collectables

} // namespace GameObjects

#endif // STELLAR_H
