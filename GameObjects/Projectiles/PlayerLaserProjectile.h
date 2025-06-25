#ifndef PLAYERLASERPROJECTILE_H
#define PLAYERLASERPROJECTILE_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class PlayerLaserProjectile : public Projectile {
public:
  PlayerLaserProjectile(int damage = 1,
                        std::unordered_set<ProjectileProperty> properties = {});
  std::unique_ptr<Projectile> clone() const override;
  static void registerPixmaps();

  // GameObject interface
public:
  void initializeObjectType() override;
  void initializeSounds() override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // PLAYERLASERPROJECTILE_H
