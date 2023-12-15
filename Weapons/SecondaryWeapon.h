#ifndef WEAPONS_SECONDARYWEAPON_H
#define WEAPONS_SECONDARYWEAPON_H

#include "Weapon.h"

namespace Weapons {

class SecondaryWeapon : public Weapon {

  // Weapon interface
public:
  std::unique_ptr<Weapon> clone() const override;

  // Weapon interface
public:
  QString getHudImagePath() const override;
};

} // namespace Weapons

#endif // WEAPONS_SECONDARYWEAPON_H
