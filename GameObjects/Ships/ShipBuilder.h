#ifndef GAMEOBJECTS_SHIPS_SHIPBUILDER_H
#define GAMEOBJECTS_SHIPS_SHIPBUILDER_H

#include "GameObjects/Ships/Ship.h"
#include <memory>

namespace GameObjects {
namespace Ships {

class ShipBuilder {
public:
  ShipBuilder();
  template <typename ShipType> ShipBuilder &createShip() {
      m_ship = std::make_unique<ShipType>(0, 0, Transform());
    m_ship->setMovementStrategy(Game::Movement::StationaryMovementStrategy());
    return *this;
  }

  ShipBuilder &withObjectType(const ObjectType objectType);
  ShipBuilder &withPosition(const Transform &transform);
  ShipBuilder &withSpeed(float speed);
  ShipBuilder &withHealth(float health);
  ShipBuilder &withEnergy(float energy);
  ShipBuilder &withEnergyRegenerationRate(float energyRegenerationRate);
  ShipBuilder &
  withMovementStrategy(const Game::Movement::MovementStrategy &strategy);
  ShipBuilder &withGrahpics(const RenderDataMap renderDataMap);
  ShipBuilder &withSpawnSound(const Game::Audio::SoundInfo spawnSound);
  ShipBuilder &
  withDestructionSound(const Game::Audio::SoundInfo destructionSound);
  std::unique_ptr<Ship> build();

private:
  std::unique_ptr<Ship> m_ship;
};

} // namespace Ships
} // namespace GameObjects

#endif // GAMEOBJECTS_SHIPS_SHIPBUILDER_H
