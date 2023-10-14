#include "GameState.h"
#include <sstream>
#include <iostream>

namespace Game {

GameState::GameState(QObject *parent)
    : QObject(parent)
{
    m_playersShipStartSpeed = 500;
}

void GameState::initialize()
{
    this->initMovementConstrains();
    this->initPlayerShip();
    //this->initEnemyShips();
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object)
{
    m_gameObjects.push_back(object);
    emit objectAdded(object);
}

void GameState::removeGameObject(std::shared_ptr<GameObjects::GameObject> object)
{
    m_gameObjects.remove(object);
}

void GameState::setSize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

void GameState::update(float deltaTimeInSeconds)
{
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

const std::list<std::shared_ptr<GameObjects::GameObject>>& GameState::gameObjects() const
{
    return m_gameObjects;
}

void GameState::initPlayerShip()
{
    GameObjects::Position pos(m_windowWidth / 2, m_maxY, m_minX, m_maxX, m_minY, m_maxY);
    std::shared_ptr<GameObjects::PlayerShip> playerShip = std::make_shared<GameObjects::PlayerShip>(100, m_playersShipStartSpeed, 3, pos);
    playerShip->initialize();
    m_playerShip = playerShip;

    connect(playerShip.get(), &GameObjects::PlayerShip::projectileShot, this, &GameState::onProjectileShot);

    this->addGameObject(std::move(playerShip));
}

void GameState::initEnemyShips()
{
    int rows = 1;
    int cols = 1;
    int width = m_maxX - m_minX;
    int height = m_maxY - m_minY - 300;
    int xSpacing = width / (cols + 1);
    int ySpacing = height / (rows + 1);
    qDebug() << "Initializing" << rows*cols << "enemy ships.";
    for (int j = 1; j <= rows; j++) {
        for (int i = 1; i <= cols; i++) {
            GameObjects::Position pos(m_minX + i * xSpacing, m_minY + j * ySpacing, m_minX, m_maxX, m_minY, m_maxY);
            std::shared_ptr<GameObjects::EnemyShip> enemyShip = std::make_shared<GameObjects::EnemyShip>(10, 100, 1, pos);
            enemyShip->initialize();
            enemyShip->setMovementStrategy(Game::Movement::CircularMovementStrategy(100, 1));
            this->addGameObject(enemyShip);
        }
    }
}


void GameState::initMovementConstrains()
{
    m_minX = 5;
    m_maxX = m_windowWidth - 25;
    m_minY = 5;
    m_maxY = m_windowHeight - 50;
}

const std::shared_ptr<GameObjects::PlayerShip> &GameState::playerShip() const
{
    return m_playerShip;
}
}
