#include "GameState.h"
#include "GameObjects/Projectiles/Vortex.h"
#include "GameObjects/Projectiles/WaveOfDestruction.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/SecondaryWeapon.h"

namespace Game {
namespace Core {
GameState::GameState(QObject *parent)
    : QObject(parent), m_playerShipDeletedFromScene(false) {
  m_playersShipStartSpeed = 500;
}

void GameState::createPlayerShip() {
  m_playerShip = std::make_unique<GameObjects::Ships::PlayerShip>(
      m_playersShipStartSpeed, GameObjects::Position(0, 0));
}

void GameState::initialize() {
  initMovementConstrains();
  initPlayerShip();
  m_stellarTokens = 0;
  m_enemyShipsReachedBottomLimit = 0;
  m_enemyShipCount = 0;
}

void GameState::deinitialize() {
  m_playerShip = nullptr;
  m_gameObjects.clear();
  m_magneticGameObjectMap.clear();

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
  emit objectAdded(object->getGraphicsItem());
}

void GameState::setSize(int width, int height) {
  m_windowWidth = width;
  m_windowHeight = height;
}

void GameState::update(float deltaTimeInSeconds) {
  for (size_t i = 0; i < m_gameObjects.size();) {
    GameObjects::UpdateContext ctx{deltaTimeInSeconds, m_playerShip.get(),
                                   m_magneticGameObjectMap};
    m_gameObjects[i]->update(ctx);
    if (m_gameObjects[i]->shouldBeDeleted()) {
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
  connect(m_playerShip.get(),
          &GameObjects::Ships::PlayerShip::stellarTokenCollected, this,
          &GameState::onStellarTokenCollected);
  connect(m_playerShip.get(), &GameObjects::GameObject::objectDestroyed, this,
          &GameState::onPlayerShipDestroyed);

  m_playerShip->initialize();
  GameObjects::Position pos(m_windowWidth / 2, m_maxY, m_minX, m_maxX, m_minY,
                            m_maxY);
  m_playerShip->setPosition(pos);

  std::unique_ptr<GameObjects::Projectiles::Projectile> vortexProjectile =
      m_projectileBuilder.createProjectile<GameObjects::Projectiles::Vortex>()
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withDamage(0)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(1000, -1))
          .build();

  std::unique_ptr<GameObjects::Projectiles::Projectile>
      waveOfDestructionProjectile =
          m_projectileBuilder
              .createProjectile<GameObjects::Projectiles::WaveOfDestruction>()
              .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
              .withDamage(1000)
              .withMovementStrategy(
                  Game::Movement::VerticalMovementStrategy(250, -1))
              .build();

  std::unique_ptr<Weapons::Weapon> waveOfDestruction =
      m_weaponBuilder.createWeapon<Weapons::SecondaryWeapon>()
          .withProjectile(std::move(waveOfDestructionProjectile))
          .withWeaponCooldownMs(5000)
          .withEnergyConsuption(200)
          .build();

  std::unique_ptr<Weapons::Weapon> vortex =
      m_weaponBuilder.createWeapon<Weapons::SecondaryWeapon>()
          .withProjectile(std::move(vortexProjectile))
          .withWeaponCooldownMs(5000)
          .withEnergyConsuption(300)
          .build();

  std::unique_ptr<GameObjects::Projectiles::Projectile> primaryProjectile =
      m_projectileBuilder
          .createProjectile<GameObjects::Projectiles::Projectile>()
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withDamage(1)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(1000, -1))
          //.withProperty(GameObjects::Projectiles::ProjectileProperty::PIERCING)
          .withGrahpics(GameObjects::PixmapData{
              QPointF(30, 30), ":/Images/player_laser_projectile.png", "", ""})
          .withSpawnSound(
              Audio::SoundInfo({true, Game::Audio::SoundEffect::LASER}))
          .build();

  // Create the primary weapon using WeaponBuilder
  std::unique_ptr<Weapons::Weapon> weapon =
      m_weaponBuilder.createWeapon<Weapons::PrimaryWeapon>()
          .withProjectile(std::move(primaryProjectile))
          .withWeaponCooldownMs(0)
          .build();

  // Clone the primary weapon and modify the projectile for the second weapon
  std::unique_ptr<Weapons::Weapon> secondWeapon =
      m_weaponBuilder.clone()
          .withProjectile(
              m_projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, -1, 80))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the third weapon
  std::unique_ptr<Weapons::Weapon> thirdWeapon =
      m_weaponBuilder.clone()
          .withProjectile(
              m_projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, -80))
                  .build())
          .build();

  m_playerShip->addPrimaryWeapon(std::move(weapon));
  m_playerShip->addPrimaryWeapon(std::move(secondWeapon));
  m_playerShip->addPrimaryWeapon(std::move(thirdWeapon));
  m_playerShip->setSecondaryWeapon(std::move(waveOfDestruction), 0);
  m_playerShip->setSecondaryWeapon(std::move(vortex), 1);
  m_playerShip->setMaxEnergy(1000);
  m_playerShip->setMaxHealth(50);
  m_playerShip->fullyRestoreEnergy();
  m_playerShip->fullyRestoreHealth();
  m_playerShip->setEnergyRegenerationRate(5);

  addGameObject(m_playerShip);
}

void GameState::initMovementConstrains() {
  m_minX = 0;
  m_maxX = m_windowWidth * 0.98;
  m_minY = 0;
  m_maxY = m_windowHeight * 0.865;
}

GameObjects::Ships::PlayerShip *GameState::playerShip() const {
  return m_playerShip.get();
}

const unsigned &GameState::stellarTokens() const { return m_stellarTokens; }

int GameState::enemyShipsReachedBottomLimit() const {
  return m_enemyShipsReachedBottomLimit;
}

int GameState::enemyShipCount() const { return m_enemyShipCount; }
} // namespace Core
} // namespace Game
