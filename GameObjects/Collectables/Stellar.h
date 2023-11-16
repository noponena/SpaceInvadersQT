#ifndef STELLAR_H
#define STELLAR_H

#include "Collectable.h"

#include <bits/shared_ptr.h>

namespace GameObjects {
namespace Collectables {

class Stellar : public Collectable {
public:
  Stellar(const Position &position);

  // GameObject interface
public:
  void collideWith(GameObject &other) override;

  // GameObject interface
protected:
  void initializeGraphics() override;
  void initializeSounds() override;
};
} // namespace Collectables

} // namespace GameObjects

#endif // STELLAR_H
