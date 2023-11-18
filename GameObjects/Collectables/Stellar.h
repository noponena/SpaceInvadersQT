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
  void initializeGraphics() override;
  void initializeSounds() override;
  void initializeObjectType() override;
};
} // namespace Collectables

} // namespace GameObjects

#endif // STELLAR_H
