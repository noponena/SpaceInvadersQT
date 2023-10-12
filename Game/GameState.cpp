#include "GameState.h"
#include <sstream>
#include <iostream>

namespace Game {

GameState::GameState(QObject *parent)
    : QObject(parent)
{
    m_playersShipStartSpeed = 0.5;
}

void GameState::initialize()
{
    this->initMovementConstrains();
    this->initPlayerShip();
    this->initEnemyShips();
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object)
{
    //std::lock_guard<std::mutex> lock(m_mutex);
    m_gameObjects.push_back(object);
    emit objectAdded(object);
}

void GameState::removeGameObject(std::shared_ptr<GameObjects::GameObject> object)
{
    //std::lock_guard<std::mutex> lock(m_mutex);
    m_gameObjects.remove(object);
}

void GameState::setSize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
}

void GameState::update(int deltaTime)
{
    //qDebug() << "gameObjects:" << m_gameObjects.size();
    auto it = m_gameObjects.begin();
    while (it != m_gameObjects.end()) {
        (*it)->update(deltaTime);
        if ((*it)->shouldBeDeleted()) {
            //std::lock_guard<std::mutex> lock(m_mutex);
            emit objectDeleted(*it);
            it = m_gameObjects.erase(it);
        } else {
            ++it;
        }
    }
}

const std::list<std::shared_ptr<GameObjects::GameObject>>& GameState::gameObjects() const
{
    //std::lock_guard<std::mutex> lock(m_mutex);
    return m_gameObjects;
}

void GameState::initPlayerShip()
{
    GameObjects::Position pos(m_windowWidth / 2, m_maxY, m_minX, m_maxX, m_minY, m_maxY);
    std::shared_ptr<GameObjects::PlayerShip> playerShip = std::make_shared<GameObjects::PlayerShip>(100, m_playersShipStartSpeed, 100, pos);
    playerShip->initialize();
    m_playerShip = playerShip;

    connect(playerShip.get(), &GameObjects::PlayerShip::laserShot, this, &GameState::onLaserShot);

    this->addGameObject(std::move(playerShip));
}

void GameState::initEnemyShips()
{
    int rows = 20;
    int cols = 40;
    //int rows = 5;
    //int cols = 5;
    qDebug() << "Initializing" << rows*cols << "enemy ships.";
    for (int j = 1; j <= rows; j++) {
        for (int i = 1; i <= cols; i++) {
            GameObjects::Position pos(i * 45 + 0, j * 40 + m_minY + 20, m_minX, m_maxX, m_minY, m_maxY);
            std::shared_ptr<GameObjects::EnemyShip> enemyShip = std::make_shared<GameObjects::EnemyShip>(10, 0.08, 1, pos);
            enemyShip->initialize();
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

std::mutex &GameState::mutex()
{
    return m_mutex;
}

}
