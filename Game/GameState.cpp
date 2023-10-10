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
    this->initEnemyShip();
}

void GameState::addGameObject(std::shared_ptr<GameObjects::GameObject> object)
{
    m_gameObjects.push_back(std::move(object));
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

void GameState::update(int deltaTime)
{
    auto it = m_gameObjects.begin();
    while (it != m_gameObjects.end()) {
        (*it)->update(deltaTime);
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
    std::shared_ptr<GameObjects::PlayerShip> playerShip = std::make_shared<GameObjects::PlayerShip>(100, m_playersShipStartSpeed, 1, pos);
    m_playerShip = playerShip;

    connect(playerShip.get(), &GameObjects::PlayerShip::laserShot, this, &GameState::onLaserShot);

    this->addGameObject(std::move(playerShip));
}

void GameState::initEnemyShip()
{
    for (int i = 1; i <= 1; i++) {
        GameObjects::Position pos(i * 900, m_minY + 50, m_minX, m_maxX, m_minY, m_maxY);
        std::shared_ptr<GameObjects::EnemyShip> enemyShip = std::make_shared<GameObjects::EnemyShip>(1, 0.08, 1, pos);
        this->addGameObject(enemyShip);
    }
}


void GameState::initMovementConstrains()
{
    m_minX = 5;
    m_maxX = m_windowWidth - 25;
    m_minY = 5;
    m_maxY = m_windowHeight - 50;
}

void GameState::addLaser(const std::shared_ptr<GameObjects::Laser> &laser)
{
    this->addGameObject(laser);
    emit laserAdded(laser);
}

const std::shared_ptr<GameObjects::PlayerShip> &GameState::playerShip() const
{
    return m_playerShip;
}

}
