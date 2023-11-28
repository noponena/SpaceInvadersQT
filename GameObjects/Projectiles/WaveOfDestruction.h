#ifndef GAMEOBJECTS_PROJECTILES_WAVEOFDESTRUCTION_H
#define GAMEOBJECTS_PROJECTILES_WAVEOFDESTRUCTION_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {

class WaveOfDestruction : public Projectile {
public:
  WaveOfDestruction();

  // GameObject interface
public:
  bool shouldBeDeleted() override;

  // Projectile interface
public:
  std::unique_ptr<Projectile> clone() const override;
};

} // namespace Projectiles
} // namespace GameObjects

#endif // GAMEOBJECTS_PROJECTILES_WAVEOFDESTRUCTION_H
