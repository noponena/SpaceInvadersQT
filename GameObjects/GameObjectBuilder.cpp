#include "GameObjectBuilder.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "GameObjects/Projectiles/Projectile.h"

namespace GameObjects {
using FactoryFunc = std::function<std::unique_ptr<GameObject>(const Config::GameContext&)>;
const std::unordered_map<ObjectType, FactoryFunc>& GetGameObjectRegistry() {
    static std::unordered_map<ObjectType, FactoryFunc> registry = {{ObjectType::PLAYER_SHIP, [](const Config::GameContext& ctx){
             return std::make_unique<Ships::PlayerShip>(ctx);
         }},

        {ObjectType::ENEMY_SHIP, [](const Config::GameContext& ctx){
             return std::make_unique<Ships::EnemyShip>(ctx);
         }},

        {ObjectType::PROJECTILE, [](const Config::GameContext& ctx){
             return std::make_unique<Projectiles::Projectile>(ctx);
         }},};
    return registry;
}


GameObjectBuilder &GameObjectBuilder::setConcreteType(ObjectType type)
{
    m_concreteType = type;
    return *this;
}

GameObjectBuilder& GameObjectBuilder::withTransform(const Transform& transform) {
    m_transform = transform;
    return *this;
}

GameObjectBuilder& GameObjectBuilder::withMovementStrategy(const Game::Movement::MovementStrategy& strategy) {
    m_movementStrategy = strategy;
    return *this;
}

GameObjectBuilder& GameObjectBuilder::withSpawnSound(const Game::Audio::SoundInfo& info) {
    m_spawnSoundInfo = info;
    return *this;
}

GameObjectBuilder& GameObjectBuilder::withDestructionSound(const Game::Audio::SoundInfo& info) {
    m_destructionSoundInfo = info;
    return *this;
}

GameObjectBuilder& GameObjectBuilder::withGraphics(const RenderDataMap& renderDataMap) {
    m_renderDataMap = renderDataMap;
    return *this;
}

std::unique_ptr<GameObject> GameObjectBuilder::build(const Config::GameContext &ctx) const
{
    auto& registry = GetGameObjectRegistry();
    auto it = registry.find(m_concreteType);
    if (it == registry.end())
        throw std::runtime_error("This object type is an abstraction and cannot be built!");

    auto obj = it->second(ctx);

    if (!obj)
        throw std::runtime_error("Failed to create GameObject!");

    obj->setGameContext(ctx);
    obj->setTransform(m_transform.value_or(Transform()));
    obj->setSpawnSoundInfo(m_spawnSoundInfo.value_or(Game::Audio::SoundInfo()));
    obj->setDestructionSoundInfo(m_destructionSoundInfo.value_or(Game::Audio::SoundInfo()));
    obj->setMovementStrategy(m_movementStrategy.value_or(Game::Movement::StationaryMovementStrategy()));
    obj->setRenderDataByState(
        m_renderDataMap.value_or(RenderDataMap{{State::Normal, RenderData()}})
        );
    return obj;
}

} // namespace GameObjects
