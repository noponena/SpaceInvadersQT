#include "GameState.h"
#include "Weapons/LaserCannon.h"
#include <iostream>
#include <sstream>

namespace Game {

GameState::GameState(QObject *parent) : QObject(parent) {
  m_playersShipStartSpeed = 500;
}

void GameState::initialize() {
  this->initMovementConstrains();
  this->initPlayerShip();
//  for (int i = 0; i < 1; i++)
//    this->initEnemyShips();
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object) {
  connect(object.get(), &GameObjects::GameObject::objectCreated,
          this, &GameState::onObjectCreated);
  m_gameObjects.push_back(object);
  emit objectAdded(object);
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
    (*it)->update(deltaTimeInSeconds);
    if ((*it)->shouldBeDeleted()) {
      emit objectDeleted(*it);
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
  std::shared_ptr<GameObjects::Ships::PlayerShip> playerShip =
      std::make_shared<GameObjects::Ships::PlayerShip>(
          100, m_playersShipStartSpeed, pos);
  playerShip->initialize();
  playerShip->setWeapon(std::make_unique<Weapons::LaserCannon>(
      1000, Game::Movement::VerticalMovementStrategy(500, -1)));
  m_playerShip = playerShip;
  this->addGameObject(std::move(playerShip));
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
      std::shared_ptr<GameObjects::Ships::EnemyShip> enemyShip =
          std::make_shared<GameObjects::Ships::EnemyShip>(10, 100, pos);
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

const std::shared_ptr<GameObjects::Ships::PlayerShip> &
GameState::playerShip() const {
  return m_playerShip;
}
} // namespace Game
