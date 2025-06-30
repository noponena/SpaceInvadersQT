#include "GameObjectBuilder.h"
#include "GameObjects/Collectables/Health.h"
#include "GameObjects/Collectables/Stellar.h"
#include "GameObjects/Projectiles/Projectile.h"
#include "GameObjects/Projectiles/Vortex.h"
#include "GameObjects/Projectiles/WaveOfDestruction.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Utils/Utils.h"

namespace GameObjects {
using FactoryFunc = std::function<std::unique_ptr<GameObject>(const Config::GameContext&)>;
const std::unordered_map<ConcreteType, FactoryFunc>& GetGameObjectRegistry() {
    static std::unordered_map<ConcreteType, FactoryFunc> registry = {{ConcreteType::PLAYER_SHIP, [](const Config::GameContext& ctx){
             return std::make_unique<Ships::PlayerShip>(ctx);
         }},

        {ConcreteType::ENEMY_SHIP, [](const Config::GameContext& ctx){
             return std::make_unique<Ships::EnemyShip>(ctx);
         }},

        {ConcreteType::PROJECTILE, [](const Config::GameContext& ctx){
             return std::make_unique<Projectiles::Projectile>(ctx);
         }},

        {ConcreteType::VORTEX, [](const Config::GameContext& ctx){
             return std::make_unique<Projectiles::Vortex>(ctx);
         }},

        {ConcreteType::WAVE_OF_DESTRUCTION, [](const Config::GameContext& ctx){
             return std::make_unique<Projectiles::WaveOfDestruction>(ctx);
         }},

        {ConcreteType::STELLAR_COIN, [](const Config::GameContext& ctx){
             return std::make_unique<Collectables::Stellar>(ctx);
         }},

        {ConcreteType::HEALTH, [](const Config::GameContext& ctx){
             return std::make_unique<Collectables::Health>(ctx);
         }},
    };
    return registry;
}

GameObjectBuilder &GameObjectBuilder::setConcreteType(ConcreteType type)
{
    m_concreteType = type;
    return *this;
}

GameObjectBuilder &GameObjectBuilder::withObjectType(ObjectType type)
{
    m_objectType = type;
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

    const QString concreteTypeStr = QString::fromStdString(Utils::concreteTypeToString(m_concreteType));

    obj->setGameContext(ctx);
    obj->addObjectType(m_objectType);

    if (m_destructionSoundInfo.has_value())
        obj->setDestructionSoundInfo(m_destructionSoundInfo.value());
    if (m_spawnSoundInfo.has_value())
        obj->setSpawnSoundInfo(m_spawnSoundInfo.value());
    if (m_transform.has_value())
        obj->setTransform(m_transform.value());
    if (m_movementStrategy.has_value())
        obj->setMovementStrategy(m_movementStrategy.value());
    if (m_renderDataMap.has_value())
        obj->setRenderDataByState(
            m_renderDataMap.value()
            );
    qDebug() << "[GameObjectBuilder] Built a game object of concrete type" << concreteTypeStr << "and collider box" << obj->transform().colliderSize;
    return obj;
}

} // namespace GameObjects
