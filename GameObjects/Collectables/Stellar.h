#ifndef STELLAR_H
#define STELLAR_H

#include "Collectable.h"

namespace GameObjects {
namespace Collectables {

class Stellar : public Collectable {
public:
  Stellar(const Position &position);

  // GameObject interface
protected:
  void initializeSounds() override;
  void initializeObjectType() override;

  // GameObject interface
public:
  std::unique_ptr<GameObject> clone() const override;
};
} // namespace Collectables

} // namespace GameObjects

#endif // STELLAR_H
