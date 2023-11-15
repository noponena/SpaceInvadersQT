#include "GameState.h"
#include "GameObjects/Projectiles/EnemyLaserBeam.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "Weapons/PrimaryWeapon.h"
#include <iostream>
#include <sstream>

namespace Game {

GameState::GameState(QObject *parent)
    : QObject(parent), m_playerShipDeletedFromScene(false) {
  m_playersShipStartSpeed = 500;
}

void GameState::initialize() {
  this->initMovementConstrains();
  this->initPlayerShip();
  m_stellarTokens = 0;
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object) {
  connect(object.get(), &GameObjects::GameObject::objectCreated, this,
          &GameState::onObjectCreated);
  m_gameObjects.emplace_back(object);
  emit objectAdded(object->getGraphicsItem());
}

void GameState::removeGameObject(
    std::shared_ptr<GameObjects::GameObject> object) {
  m_gameObjects.remove(object);
}

void GameState::setSize(int width, int height) {
  m_windowWidth = width;
  m_windowHeight = height;
}

void GameState::update(float deltaTimeInSeconds) {
  auto it = m_gameObjects.begin();
  while (it != m_gameObjects.end()) {
    (*it)->update({deltaTimeInSeconds, m_playerShip});
    if ((*it)->shouldBeDeleted()) {
      emit objectDeleted(it->get()->getGraphicsItem());
      it = m_gameObjects.erase(it);
    } else {
      ++it;
    }
  }
}

const std::list<std::shared_ptr<GameObjects::GameObject>> &
GameState::gameObjects() const {
  return m_gameObjects;
}

void GameState::initPlayerShip() {

  GameObjects::Position pos(m_windowWidth / 2, m_maxY, m_minX, m_maxX, m_minY,
                            m_maxY);
  std::unique_ptr<GameObjects::Ships::PlayerShip> playerShip =
      std::make_unique<GameObjects::Ships::PlayerShip>(
          25, m_playersShipStartSpeed, pos);
  playerShip->initialize();
  connect(playerShip.get(),
          &GameObjects::Ships::PlayerShip::playerShipDestroyed, this,
          &GameState::onPlayerShipDestroyed);

  std::unique_ptr<Weapons::Weapon> weapon =
      m_weaponBuilder.createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
          .withProjectileDamage(1)
          .withProjectile(
              std::make_unique<GameObjects::Projectiles::PlayerLaserBeam>())
          .withProjectileMovementStrategy(
              Game::Movement::VerticalMovementStrategy(1000, -1))
          //.withProjectileProperty(Weapons::ProjectileProperty::PIERCING)
          .withWeaponCooldownMs(0)
          .build();

  std::unique_ptr<Weapons::Weapon> secondWeapon =
      m_weaponBuilder.cloneWeapon(weapon)
          .withProjectileMovementStrategy(
              Game::Movement::AngledMovementStrategy(1000, -1, 80))
          .build();

  std::unique_ptr<Weapons::Weapon> thirdWeapon =
      m_weaponBuilder.cloneWeapon(weapon)
          .withProjectileMovementStrategy(
              Game::Movement::AngledMovementStrategy(1000, 1, -80))
          .build();

  playerShip->addWeapon(std::move(weapon));
  playerShip->addWeapon(std::move(secondWeapon));
  playerShip->addWeapon(std::move(thirdWeapon));
  m_playerShip = playerShip.get();
  connect(m_playerShip, &GameObjects::Ships::PlayerShip::stellarTokenCollected,
          this, &GameState::onStellarTokenCollected);
  this->addGameObject(std::move(playerShip));
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
      500, 700);//m_gameState.m_minX, m_gameState.m_maxX); // define the range

  int randomX = distr(eng);
  std::unique_ptr<Weapons::Weapon> weapon =
      m_weaponBuilder.createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
          .withProjectileDamage(1)
          .withProjectile(
              std::make_unique<GameObjects::Projectiles::EnemyLaserBeam>())
          .withProjectileMovementStrategy(
              Game::Movement::VerticalMovementStrategy(500, 1))
          .withWeaponCooldownMs(0)
          .build();
  //qDebug() << "Initializing" << rows * cols << "enemy ships.";
  for (int j = 1; j <= rows; j++) {
    for (int i = 1; i <= cols; i++) {
      GameObjects::Position pos(randomX,
                                m_minY + j * ySpacing + 500, m_minX, m_maxX,
                                m_minY, m_maxY);
      std::unique_ptr<GameObjects::Ships::EnemyShip> enemyShip =
          std::make_unique<GameObjects::Ships::EnemyShip>(1, pos);
      enemyShip->initialize();
      //enemyShip->addWeapon(weapon->clone());
//        enemyShip->setMovementStrategy(Game::Movement::CircularMovementStrategy(100,
//        1));
      this->addGameObject(std::move(enemyShip));
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
} // namespace Game
