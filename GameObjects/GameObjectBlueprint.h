#pragma once

#include "Game/Movement/MovementStrategy.h"
#include "GameObject.h"
#include "GameObjectBuilder.h"
#include "Utils/Utils.h"

namespace GameObjects {
struct GameObjectBlueprint {
  std::string name;               // "FastEnemy", "PlayerShip", etc.
  GameObjects::ConcreteType type; // e.g., ObjectType::ENEMY_SHIP
  std::string variant;            // "Boss1", "Wave3", optional

  // Core gameplay parameters
  float maxHealth = 100.f;
  float speed = 0.f;
  float damage = 0.f;
  float energy = 0.f;
  float energyRegenerationRate = 0.f;

  // Movement
  std::optional<Game::Movement::MovementStrategy>
      movement; // Or just an enum for the movement type and params

  // Child/variant specific fields
  std::optional<float> bossAuraRadius;
  std::optional<std::string>
      weaponBlueprintName; // For enemies/ships with weapons

  std::unique_ptr<GameObjects::GameObject>
  instantiate(const Config::GameContext &ctx) const {
    // Log the type of object to be instantiated
    qDebug() << "[GameObjectBlueprint] Instantiating object of type:"
             << QString::fromStdString(Utils::concreteTypeToString(type));

    GameObjects::GameObjectBuilder builder;
    builder.setConcreteType(type);

    if (movement.has_value()) {
      qDebug() << "[GameObjectBlueprint] ...movement strategy present";
      builder.withMovementStrategy(movement.value());
    }

    // Child/variant-specific fields
    if (bossAuraRadius) {
      qDebug() << "[GameObjectBlueprint] ...bossAuraRadius present, value ="
               << *bossAuraRadius;
      // Example: Cast to BossEnemy and set bossAuraRadius
    }
    if (weaponBlueprintName) {
      qDebug()
          << "[GameObjectBlueprint] ...weaponBlueprintName present, value ="
          << QString::fromStdString(*weaponBlueprintName);
      // (You may want to spawn or look up the weapon by blueprint name)
    }

    // Finally, build the object (returns base class; cast if needed for child
    // types)
    auto instance = builder.build(ctx);

    qDebug() << "[GameObjectBlueprint] Instantiation complete for type:"
             << QString::fromStdString(Utils::concreteTypeToString(type));

    return instance;
  }
};
} // namespace GameObjects
