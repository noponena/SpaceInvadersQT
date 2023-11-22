#include "GameState.h"
#include "GameObjects/Projectiles/WaveOfDestruction.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/SecondaryWeapon.h"
#include "GameObjects/Projectiles/Vortex.h"
#include <iostream>
#include <sstream>

namespace Game {
namespace Core {
GameState::GameState(QObject *parent)
    : QObject(parent), m_playerShipDeletedFromScene(false) {
  m_playersShipStartSpeed = 500;
}

void GameState::initialize() {
  initMovementConstrains();
  initPlayerShip();
  m_stellarTokens = 0;
  m_collidingPairs.clear();
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object) {
  connect(object.get(), &GameObjects::GameObject::objectCreated, this,
          &GameState::onObjectCreated);
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
      m_gameObjects[i]->update({deltaTimeInSeconds, m_playerShip, m_magneticGameObjectMap});
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

  GameObjects::Position pos(m_windowWidth / 2, m_maxY, m_minX, m_maxX, m_minY,
                            m_maxY);
  qDebug() << "playerShip x:" << m_windowWidth / 2 << "playerShip y:" << m_maxY;
  std::unique_ptr<GameObjects::Ships::PlayerShip> playerShip =
      std::make_unique<GameObjects::Ships::PlayerShip>(
          2500, m_playersShipStartSpeed, pos);
  playerShip->initialize();
  connect(playerShip.get(), &GameObjects::GameObject::objectDestroyed, this,
          &GameState::onPlayerShipDestroyed);

//  std::unique_ptr<GameObjects::Projectiles::Projectile> secondaryProjectile =
//      m_projectileBuilder
//          .createProjectile(std::make_unique<GameObjects::Projectiles::Vortex>())
//          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
//          .withDamage(0)
//          .withMovementStrategy(Game::Movement::VerticalMovementStrategy(1000, -1))
//          .build();

  std::unique_ptr<GameObjects::Projectiles::Projectile> secondaryProjectile =
      m_projectileBuilder
          .createProjectile(std::make_unique<GameObjects::Projectiles::WaveOfDestruction>())
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withGrahpics(GameObjects::PixmapData{QPointF(250, 20), ":/Images/wave.png", "", false})
          .withDamage(1000)
          .withMovementStrategy(Game::Movement::VerticalMovementStrategy(250, -1))
          .build();

  std::unique_ptr<Weapons::Weapon> secondaryWeapon =
      m_weaponBuilder
          .createWeapon(std::make_unique<Weapons::SecondaryWeapon>())
          .withProjectile(std::move(secondaryProjectile))
          .withWeaponCooldownMs(100)
          .build();

  std::unique_ptr<GameObjects::Projectiles::Projectile> primaryProjectile =
      m_projectileBuilder
          .createProjectile(std::make_unique<GameObjects::Projectiles::Projectile>())
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withDamage(1)
          .withMovementStrategy(Game::Movement::VerticalMovementStrategy(1000, -1))
          //.withProperty(GameObjects::Projectiles::ProjectileProperty::PIERCING)
          .withGrahpics(GameObjects::PixmapData{QPointF(30, 30), ":/Images/player_laser_projectile.png", ""})
          .withSpawnSound(Audio::SoundInfo({true, Game::Audio::SoundEffect::LASER}))
          .build();

  // Create the primary weapon using WeaponBuilder
  std::unique_ptr<Weapons::Weapon> weapon =
      m_weaponBuilder
          .createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
          .withProjectile(std::move(primaryProjectile))
          .withWeaponCooldownMs(0)
          .build();

  // Clone the primary weapon and modify the projectile for the second weapon
  std::unique_ptr<Weapons::Weapon> secondWeapon =
      m_weaponBuilder.clone()
          .withProjectile(
              m_projectileBuilder
                  .withMovementStrategy(Game::Movement::AngledMovementStrategy(1000, -1, 80))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the third weapon
  std::unique_ptr<Weapons::Weapon> thirdWeapon =
      m_weaponBuilder.clone()
          .withProjectile(
              m_projectileBuilder
                  .withMovementStrategy(Game::Movement::AngledMovementStrategy(1000, 1, -80))
                  .build())
          .build();
  
  playerShip->addPrimaryWeapon(std::move(weapon));
  playerShip->addPrimaryWeapon(std::move(secondWeapon));
   playerShip->addPrimaryWeapon(std::move(thirdWeapon));
  playerShip->addSecondaryWeapon(std::move(secondaryWeapon));
  m_playerShip = playerShip.get();
  connect(m_playerShip, &GameObjects::Ships::PlayerShip::stellarTokenCollected,
          this, &GameState::onStellarTokenCollected);
  addGameObject(std::move(playerShip));
}

void GameState::initEnemyShips() {
  int rows = 1;
  int cols = 1;
  int width = m_maxX - m_minX;
  int height = m_maxY - m_minY - 300;
  int xSpacing = width / (cols + 1);
  int ySpacing = height / (rows + 1);
  std::random_device rd;  // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator
  std::uniform_int_distribution<> distr(
      100, 1500);

  int randomX = distr(eng);
  int randomY = distr(eng);
  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      m_projectileBuilder
          .createProjectile(std::make_unique<GameObjects::Projectiles::Projectile>())
          .withDamage(1)
          .withMovementStrategy(Game::Movement::VerticalMovementStrategy(500, 1))
          .withObjectType(GameObjects::ObjectType::ENEMY_PROJECTILE)
          .build();

  // Then, create the weapon using WeaponBuilder
  std::unique_ptr<Weapons::Weapon> weapon =
      m_weaponBuilder
          .createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
          .withProjectile(std::move(projectile))
          .withWeaponCooldownMs(0)
          .build();

  // qDebug() << "Initializing" << rows * cols << "enemy ships.";
  for (int j = 1; j <= rows; j++) {
    for (int i = 1; i <= cols; i++) {
      GameObjects::Position pos(randomX, randomY, m_minX,
                                m_maxX, m_minY, m_maxY);
      std::unique_ptr<GameObjects::Ships::EnemyShip> enemyShip =
          std::make_unique<GameObjects::Ships::EnemyShip>(1, pos);
      enemyShip->initialize();
      // enemyShip->addWeapon(weapon->clone());
      //        enemyShip->setMovementStrategy(Game::Movement::CircularMovementStrategy(100,
      //        1));
      addGameObject(std::move(enemyShip));
    }
  }
}

void GameState::initMovementConstrains() {
  m_minX = 0;
  m_maxX = m_windowWidth * 0.98;
  m_minY = 0;
  m_maxY = m_windowHeight * 0.95;
}

GameObjects::Ships::PlayerShip *GameState::playerShip() const {
  return m_playerShip;
}

const unsigned &GameState::stellarTokens() const { return m_stellarTokens; }

std::mutex &GameState::mutex()
{
  return m_mutex;
}
} // namespace Core
} // namespace Game
