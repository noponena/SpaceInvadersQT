#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QObject>
#include <vector>
#include <memory>
#include <list>
#include <unordered_map>
#include "GameObjects/GameObject.h"
#include "GameObjects/Projectiles/Laser.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "GameObjects/Ships/EnemyShip.h"

namespace Game {

class GameState : public QObject
{
    Q_OBJECT
public:
    GameState(QObject *parent = nullptr);

    void initialize();
    void addGameObject(std::shared_ptr<GameObjects::GameObject> object);
    void removeGameObject(std::shared_ptr<GameObjects::GameObject> object);
    void setSize(int width, int height);
    void update(int deltaTime);

    const std::list<std::shared_ptr<GameObjects::GameObject>>& gameObjects() const;
    const std::shared_ptr<GameObjects::PlayerShip> &playerShip() const;
private:
    std::list<std::shared_ptr<GameObjects::GameObject>> m_gameObjects;
    int m_minX;
    int m_minY;
    int m_maxX;
    int m_maxY;
    int m_windowWidth;
    int m_windowHeight;
    float m_playersShipStartSpeed;
    void initPlayerShip();
    void initEnemyShip();
    void initMovementConstrains();
    void addLaser(const std::shared_ptr<GameObjects::Laser>& laser);
    std::shared_ptr<GameObjects::PlayerShip> m_playerShip;

signals:
    void laserAdded(const std::shared_ptr<GameObjects::Laser>& laser);
    void enemyAdded(const std::shared_ptr<GameObjects::EnemyShip>& laser);
    void objectDeleted(const std::shared_ptr<GameObjects::GameObject>& object);

public slots:
    void onLaserShot(const std::shared_ptr<GameObjects::Laser>& laser) {
        addLaser(laser);
    }
};


}



#endif // GAMESTATE_H
