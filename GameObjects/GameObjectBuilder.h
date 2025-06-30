#pragma once

#include "Game/Audio/SoundInfo.h"
#include "Game/Movement/MovementStrategy.h"
#include "GameObject.h"
#include <memory>
#include <optional>

namespace GameObjects {

// Main builder
class GameObjectBuilder {
public:
    GameObjectBuilder& setConcreteType(ConcreteType type);
    GameObjectBuilder& withObjectType(ObjectType type);
    GameObjectBuilder& withTransform(const Transform& transform);
    GameObjectBuilder& withMovementStrategy(const Game::Movement::MovementStrategy& strategy);
    GameObjectBuilder& withSpawnSound(const Game::Audio::SoundInfo& info);
    GameObjectBuilder& withDestructionSound(const Game::Audio::SoundInfo& info);
    GameObjectBuilder& withGraphics(const RenderDataMap& renderDataMap);

    std::unique_ptr<GameObject> build(const Config::GameContext &ctx) const;

    template<typename T>
    std::unique_ptr<T> buildAs(Config::GameContext &ctx) const {
        auto base = build(ctx);
        auto raw = dynamic_cast<T*>(base.release());
        if (!raw)
            throw std::runtime_error("buildAs: Type mismatch!");
        return std::unique_ptr<T>(raw);
    }

private:
    ConcreteType m_concreteType;
    ObjectType m_objectType;
    std::optional<Transform> m_transform;
    std::optional<Game::Movement::MovementStrategy> m_movementStrategy;
    std::optional<Game::Audio::SoundInfo> m_spawnSoundInfo;
    std::optional<Game::Audio::SoundInfo> m_destructionSoundInfo;
    std::optional<RenderDataMap> m_renderDataMap;
};

} // namespace GameObjects
