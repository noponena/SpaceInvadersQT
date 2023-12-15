#ifndef WEAPONS_PRIMARYWEAPON_H
#define WEAPONS_PRIMARYWEAPON_H

#include "Weapon.h"

namespace Weapons {

class PrimaryWeapon : public Weapon {

  // Weapon interface
public:
  std::unique_ptr<Weapon> clone() const override;

  // Weapon interface
public:
  QString getHudImagePath() const override;
};

} // namespace Weapons

#endif // WEAPONS_PRIMARYWEAPON_H
