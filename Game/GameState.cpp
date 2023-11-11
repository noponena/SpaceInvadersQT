#include "GameState.h"
#include "Weapons/PrimaryWeapon.h"
#include "GameObjects/Ships/EnemyShip.h"
#include <iostream>
#include <sstream>

namespace Game {

GameState::GameState(QObject *parent) : QObject(parent), m_playerShipDeletedFromScene(false) {
  m_playersShipStartSpeed = 500;
}

void GameState::initialize() {
  this->initMovementConstrains();
  this->initPlayerShip();
  m_stellarTokens = 0;
//  for (int i = 0; i < 1; i++)
//    this->initEnemyShips();
}

void GameState::addGameObject(GameObjects::GameObject *object) {
  connect(object, &GameObjects::GameObject::objectCreated,
          this, &GameState::onObjectCreated);
  m_gameObjects.emplace_back(object);
  emit objectAdded(object);
}

void GameState::removeGameObject(
    std::unique_ptr<GameObjects::GameObject> object) {
  m_gameObjects.remove(object);
}

void GameState::setSize(int width, int height) {
  m_windowWidth = width;
  m_windowHeight = height;
}

void GameState::update(float deltaTimeInSeconds) {
  GameObjects::UpdateContext context{ deltaTimeInSeconds, *m_playerShip };
  if (!m_playerShipDeletedFromScene && m_playerShip->isDestroyed()) {
      emit objectDeleted(m_playerShip);
      m_playerShipDeletedFromScene = true;
  }
  auto it = m_gameObjects.begin();
  while (it != m_gameObjects.end()) {
    (*it)->update(context);
    if ((*it)->shouldBeDeleted()) {
      emit objectDeleted(it->get());
      it = m_gameObjects.erase(it);
    } else {
      ++it;
    }
  }
}

const std::list<std::unique_ptr<GameObjects::GameObject> > &GameState::gameObjects() const {
  return m_gameObjects;
}

void GameState::initPlayerShip() {

  GameObjects::Position pos(m_windowWidth / 2, m_maxY, m_minX, m_maxX, m_minY,
                            m_maxY);
  GameObjects::Ships::PlayerShip *playerShip =
      new GameObjects::Ships::PlayerShip(
          10, m_playersShipStartSpeed, pos);
  playerShip->initialize();
  std::unique_ptr<Weapons::PrimaryWeapon<GameObjects::Projectiles::LaserBeam>>
      weapon = std::make_unique<Weapons::PrimaryWeapon<GameObjects::Projectiles::LaserBeam>>(
      0, Game::Movement::VerticalMovementStrategy(1000, -1));
  //weapon->addProperty(Weapons::WeaponProperty::PIERCING);
  playerShip->setWeapon(std::move(weapon));
  m_playerShip = playerShip;
  connect(m_playerShip, &GameObjects::Ships::PlayerShip::stellarTokenCollected,
          this, &GameState::onStellarTokenCollected);
  this->addGameObject(playerShip);
}

void GameState::initEnemyShips() {
  int rows = 1;
  int cols = 1;
  int width = m_maxX - m_minX;
  int height = m_maxY - m_minY - 300;
  int xSpacing = width / (cols + 1);
  int ySpacing = height / (rows + 1);
  qDebug() << "Initializing" << rows * cols << "enemy ships.";
  for (int j = 1; j <= rows; j++) {
    for (int i = 1; i <= cols; i++) {
      GameObjects::Position pos(m_minX + i * xSpacing - 500, m_minY + j * ySpacing + 500,
                                m_minX, m_maxX, m_minY, m_maxY);
      GameObjects::Ships::EnemyShip *enemyShip =
          new GameObjects::Ships::EnemyShip(10, 100, pos);
      enemyShip->initialize();
      // enemyShip->setMovementStrategy(Game::Movement::CircularMovementStrategy(100,
      // 1));
      this->addGameObject(enemyShip);
    }
  }
}

void GameState::initMovementConstrains() {
  m_minX = 0;
  m_maxX = m_windowWidth - 70;
  m_minY = 0;
  m_maxY = m_windowHeight - 85;
}

GameObjects::Ships::PlayerShip*
GameState::playerShip() const {
    return m_playerShip;
}

const unsigned &GameState::stellarTokens() const
{
    return m_stellarTokens;
}
} // namespace Game
