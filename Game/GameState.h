#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Projectiles/LaserBeam.h"
#include "GameObjects/Ships/EnemyShip.h"
#include "GameObjects/Ships/PlayerShip.h"

namespace Game {

class GameState : public QObject {
  Q_OBJECT
public:
  GameState(QObject *parent = nullptr);

  void initialize();
  void addGameObject(std::shared_ptr<GameObjects::GameObject> object);
  void removeGameObject(std::shared_ptr<GameObjects::GameObject> object);
  void setSize(int width, int height);
  void update(float deltaTimeInSeconds);
  void initEnemyShips();

  const std::list<std::shared_ptr<GameObjects::GameObject>> &
  gameObjects() const;
  const std::shared_ptr<GameObjects::Ships::PlayerShip> &playerShip() const;

  int m_minX, m_minY, m_maxX, m_maxY, m_windowWidth, m_windowHeight;

private:
  std::list<std::shared_ptr<GameObjects::GameObject>> m_gameObjects;
  float m_playersShipStartSpeed;
  void initPlayerShip();
  void initMovementConstrains();
  void
  addLaser(const std::shared_ptr<GameObjects::Projectiles::LaserBeam> &laser);
  std::shared_ptr<GameObjects::Ships::PlayerShip> m_playerShip;

signals:
  void objectDeleted(const std::shared_ptr<GameObjects::GameObject> &object);
  void objectAdded(const std::shared_ptr<GameObjects::GameObject> &object);

public slots:
  void onProjectileShot(
      const std::shared_ptr<GameObjects::Projectiles::Projectile> &projectile) {
    this->addGameObject(projectile);
  }
};
} // namespace Game

#endif // GAMESTATE_H
