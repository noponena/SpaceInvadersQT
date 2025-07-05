#include "GameState.h"
#include "GameObjects/Projectiles/Vortex.h"
#include "GameObjects/Projectiles/WaveOfDestruction.h"
#include "GameObjects/PrototypeRegistry.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/SecondaryWeapon.h"

using ProjectilePrototypeRegistry =
    GameObjects::PrototypeRegistry<PrototypeKey, GameObjects::GameObject>;

namespace Game {
namespace Core {
GameState::GameState(Config::GameContext gameCtx, QObject *parent)
    : QObject(parent), m_gameCtx(gameCtx), m_playerShipDeletedFromScene(false) {
  m_playersShipStartSpeed = 500;
}

void GameState::createPlayerShip() {
  m_playerShip = std::make_shared<GameObjects::Ships::PlayerShip>(m_gameCtx);
  m_playerShip->setSpeed(m_playersShipStartSpeed);
}

void GameState::initialize() {
  initPlayerShip();
  m_stellarTokens = 0;
  m_enemyShipsReachedBottomLimit = 0;
  m_enemyShipCount = 0;
}

void GameState::deinitialize() {
  m_playerShip = nullptr;
  m_gameObjects.clear();
  m_magneticGameObjectMap.clear();

  // std::vector<std::shared_ptr<GameObjects::GameObject>>{}.swap(m_gameObjects);
  // std::unordered_map<std::uint64_t,
  //                    std::shared_ptr<GameObjects::GameObject>>{}.swap(m_magneticGameObjectMap);

  // disconnect(m_playerShip.get(), &GameObjects::GameObject::objectCreated,
  // this,
  //         &GameState::onObjectCreated);
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object) {
  connect(object.get(), &GameObjects::GameObject::objectCreated, this,
          &GameState::onObjectCreated);
  std::unordered_set<GameObjects::ObjectType> types = object->objectTypes();

  if (types.find(GameObjects::ObjectType::ENEMY_SHIP) != types.end()) {
    std::shared_ptr<GameObjects::Ships::EnemyShip> enemyShip =
        std::static_pointer_cast<GameObjects::Ships::EnemyShip>(object);

    connect(enemyShip.get(), &GameObjects::Ships::EnemyShip::bottomEdgeReached,
            this, &GameState::onEnemyReachedBottomEdge);

    connect(enemyShip.get(), &GameObjects::Ships::EnemyShip::enemyShipDeleted,
            this, &GameState::onEnemyShipDeleted);

    m_enemyShipCount++;
  }

  m_gameObjects.emplace_back(object);
  if (object->magnetism().isMagnetic)
    m_magneticGameObjectMap[object->id()] = object;
}

void GameState::update(float deltaTimeInSeconds) {
  for (size_t i = 0; i < m_gameObjects.size();) {
    GameObjects::UpdateContext ctx{deltaTimeInSeconds, m_playerShip.get(),
                                   m_magneticGameObjectMap};
    m_gameObjects[i]->update(ctx);
    if (m_gameObjects[i]->shouldBeDeleted()) {
#ifndef NDEBUG
      int useCount = m_gameObjects[i].use_count();
      if (useCount > 1)
        qCritical() << "More than 1 owners at the moment of deletion:"
                    << useCount;
#endif
      m_magneticGameObjectMap.erase(m_gameObjects[i]->id());
      std::swap(m_gameObjects[i], m_gameObjects.back());
      m_gameObjects.pop_back();
    } else {
      ++i;
    }
  }
}

const std::vector<std::shared_ptr<GameObjects::GameObject>> &
GameState::gameObjects() const {
  return m_gameObjects;
}

void GameState::initPlayerShip() {

  qDebug() << "Initializing player ship..";
  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::stellarTokenCollected, this,
          &GameState::onStellarTokenCollected);
  connect(m_playerShip.get(), &GameObjects::GameObject::objectDestroyed, this,
          &GameState::onPlayerShipDestroyed);

  m_playerShip->initialize();
  QVector2D posVec(m_gameCtx.screenGeometry.center().x(),
                   m_gameCtx.movementBounds.bottom());
  m_playerShip->moveAbsolute(posVec);

  auto &projectileRegistry = ProjectilePrototypeRegistry::instance();

  GameObjects::RenderDataMap renderDataMap{
      {GameObjects::State::Normal,
       GameObjects::RenderData({30, 30},
                               ":/Images/player_laser_projectile.png")}};

  // 1. Register Vortex Projectile prototype
  {
    qDebug() << "Building Vortex projectile..";
    auto vortexProjectile =
        m_gameObjectBuilder.setConcreteType(GameObjects::ConcreteType::VORTEX)
            .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
            .withMovementStrategy(
                Game::Movement::VerticalMovementStrategy(1000, -1))
            .buildAs<GameObjects::Projectiles::Vortex>(m_gameCtx);

    vortexProjectile->setDamage(0);

    qDebug() << "Vortex projectile built!";

    projectileRegistry.registerPrototype(
        {GameObjects::ObjectType::PROJECTILE, "VORTEX"},
        std::move(vortexProjectile), true);
  }

  // 2. Register Wave of Destruction Projectile prototype
  {
    qDebug() << "Building WaveOfDestruction projectile..";
    auto waveOfDestructionProjectile =
        m_gameObjectBuilder
            .setConcreteType(GameObjects::ConcreteType::WAVE_OF_DESTRUCTION)
            .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
            .withMovementStrategy(
                Game::Movement::VerticalMovementStrategy(250, -1))
            .buildAs<GameObjects::Projectiles::WaveOfDestruction>(m_gameCtx);

    waveOfDestructionProjectile->setDamage(1000);

    qDebug() << "WaveOfDestruction projectile built!";

    projectileRegistry.registerPrototype(
        {GameObjects::ObjectType::PROJECTILE, "WAVE_OF_DESTRUCTION"},
        std::move(waveOfDestructionProjectile), true);
  }

  // 3. Register Primary Projectile prototype
  {
    qDebug() << "Building primary projectile..";
    auto primaryProjectile =
        m_gameObjectBuilder
            .setConcreteType(GameObjects::ConcreteType::PROJECTILE)
            .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
            .withMovementStrategy(
                Game::Movement::VerticalMovementStrategy(1000, -1))
            .withGraphics(renderDataMap)
            .withSpawnSound(
                Audio::SoundInfo({true, Game::Audio::SoundEffect::LASER}))
            .buildAs<GameObjects::Projectiles::Projectile>(m_gameCtx);

    primaryProjectile->setDamage(1);

    qDebug() << "Primary projectile built!";

    projectileRegistry.registerPrototype(
        {GameObjects::ObjectType::PROJECTILE, "PRIMARY"},
        std::move(primaryProjectile), true);
  }

  auto vortexProjectile =
      projectileRegistry.cloneAs<GameObjects::Projectiles::Vortex>(
          {GameObjects::ObjectType::PROJECTILE, "VORTEX"});

  auto waveOfDestructionProjectile =
      projectileRegistry.cloneAs<GameObjects::Projectiles::WaveOfDestruction>(
          {GameObjects::ObjectType::PROJECTILE, "WAVE_OF_DESTRUCTION"});

  auto primaryProjectile =
      projectileRegistry.cloneAs<GameObjects::Projectiles::Projectile>(
          {GameObjects::ObjectType::PROJECTILE, "PRIMARY"});

  // Build Vortex Weapon
  auto vortex = m_weaponBuilder.createWeapon<Weapons::SecondaryWeapon>()
                    .withProjectile(std::move(vortexProjectile))
                    .withWeaponCooldownMs(5000)
                    .withEnergyConsuption(300)
                    .build();

  // Build Wave of Destruction Weapon
  auto waveOfDestruction =
      m_weaponBuilder.createWeapon<Weapons::SecondaryWeapon>()
          .withProjectile(std::move(waveOfDestructionProjectile))
          .withWeaponCooldownMs(5000)
          .withEnergyConsuption(200)
          .build();

  // Build Primary Weapon
  auto weapon = m_weaponBuilder.createWeapon<Weapons::PrimaryWeapon>()
                    .withProjectile(std::move(primaryProjectile))
                    .withWeaponCooldownMs(0)
                    .build();

  // Clone and customize for second weapon
  auto secondWeaponProjectile =
      m_gameObjectBuilder.setConcreteType(GameObjects::ConcreteType::PROJECTILE)
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withMovementStrategy(
              Game::Movement::AngledMovementStrategy(1000, 1, 10))
          .buildAs<GameObjects::Projectiles::Projectile>(m_gameCtx);

  auto secondWeapon = m_weaponBuilder.clone()
                          .withProjectile(std::move(secondWeaponProjectile))
                          .build();

  // Clone and customize for third weapon
  auto thirdWeaponProjectile =
      m_gameObjectBuilder.setConcreteType(GameObjects::ConcreteType::PROJECTILE)
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withMovementStrategy(
              Game::Movement::AngledMovementStrategy(1000, 1, -10))
          .buildAs<GameObjects::Projectiles::Projectile>(m_gameCtx);

  auto thirdWeapon = m_weaponBuilder.clone()
                         .withProjectile(std::move(thirdWeaponProjectile))
                         .build();

  m_playerShip->addPrimaryWeapon(std::move(weapon));
  m_playerShip->addPrimaryWeapon(std::move(secondWeapon));
  m_playerShip->addPrimaryWeapon(std::move(thirdWeapon));
  m_playerShip->setSecondaryWeapon(std::move(waveOfDestruction), 0);
  m_playerShip->setSecondaryWeapon(std::move(vortex), 1);
  m_playerShip->setMaxEnergy(1000);
  m_playerShip->setMaxHealth(50);
  m_playerShip->restoreEnergy();
  m_playerShip->restoreHealth();
  m_playerShip->setEnergyRegenerationRate(5);

  addGameObject(m_playerShip);
}

std::shared_ptr<GameObjects::Ships::PlayerShip> GameState::playerShip() const {
  return m_playerShip;
}

const std::uint32_t &GameState::stellarTokens() const {
  return m_stellarTokens;
}

int GameState::enemyShipsReachedBottomLimit() const {
  return m_enemyShipsReachedBottomLimit;
}

int GameState::enemyShipCount() const { return m_enemyShipCount; }
} // namespace Core
} // namespace Game
