#include "ShipBuilder.h"

namespace GameObjects {
namespace Ships {

ShipBuilder::ShipBuilder() {}

ShipBuilder &ShipBuilder::withObjectType(const ObjectType objectType) {
  if (m_ship)
    m_ship->addObjectType(objectType);
  return *this;
}

ShipBuilder &ShipBuilder::withPosition(const Position &position) {
  if (m_ship)
    m_ship->setPosition(position);
  return *this;
}

ShipBuilder &ShipBuilder::withSpeed(float speed) {
  if (m_ship)
    m_ship->setSpeed(speed);
  return *this;
}

ShipBuilder &ShipBuilder::withHealth(float health) {
  if (m_ship) {
    m_ship->setMaxHealth(health);
    m_ship->fullyRestoreHealth();
  }
  return *this;
}

ShipBuilder &ShipBuilder::withEnergy(float energy) {
  if (m_ship) {
    m_ship->setMaxEnergy(energy);
    m_ship->fullyRestoreEnergy();
  }
  return *this;
}

ShipBuilder &
ShipBuilder::withEnergyRegenerationRate(float energyRegenerationRate) {
  if (m_ship)
    m_ship->setEnergyRegenerationRate(energyRegenerationRate);
  return *this;
}

ShipBuilder &ShipBuilder::withMovementStrategy(
    const Game::Movement::MovementStrategy &strategy) {
  if (m_ship)
    m_ship->setMovementStrategy(strategy);
  return *this;
}

ShipBuilder &
ShipBuilder::withSpawnSound(const Game::Audio::SoundInfo spawnSound) {
  if (m_ship)
    m_ship->setSpawnSoundInfo(spawnSound);
  return *this;
}

ShipBuilder &ShipBuilder::withDestructionSound(
    const Game::Audio::SoundInfo destructionSound) {
  if (m_ship)
    m_ship->setDestructionSoundInfo(destructionSound);
  return *this;
}

std::unique_ptr<Ship> ShipBuilder::build() {
  return std::unique_ptr<Ship>(
      static_cast<Ship *>(this->m_ship->clone().release()));
}

ShipBuilder &ShipBuilder::withGrahpics(const PixmapData pixmapData) {
  if (m_ship)
    m_ship->setPixmapData(pixmapData);
  return *this;
}

} // namespace Ships
} // namespace GameObjects
