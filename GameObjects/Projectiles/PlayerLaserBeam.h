#ifndef PLAYERLASERBEAM_H
#define PLAYERLASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class PlayerLaserBeam : public Projectile {
public:
  PlayerLaserBeam(
      int damage = 1,
      std::unordered_set<Weapons::ProjectileProperty> properties = {});
  std::unique_ptr<Projectile> clone() const override;

  // GameObject interface
public:
  void initializeObjectType() override;
  void initializeGraphics() override;
  void initializeSounds() override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // PLAYERLASERBEAM_H
