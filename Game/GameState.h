#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Projectiles/LaserBeam.h"
#include "GameObjects/Ships/PlayerShip.h"

namespace Game {

class GameState : public QObject {
  Q_OBJECT
public:
  GameState(QObject *parent = nullptr);

  void initialize();
  void addGameObject(GameObjects::GameObject *object);
  void removeGameObject(std::unique_ptr<GameObjects::GameObject> object);
  void setSize(int width, int height);
  void update(float deltaTimeInSeconds);
  void initEnemyShips();

  const std::list<std::unique_ptr<GameObjects::GameObject>> &
  gameObjects() const;
  GameObjects::Ships::PlayerShip *playerShip() const;

  int m_minX, m_minY, m_maxX, m_maxY, m_windowWidth, m_windowHeight;

  const unsigned &stellarTokens() const;

private:
  std::list<std::unique_ptr<GameObjects::GameObject>> m_gameObjects;
  unsigned m_stellarTokens;
  float m_playersShipStartSpeed;
  void initPlayerShip();
  void initMovementConstrains();
  void
  addLaser(const std::shared_ptr<GameObjects::Projectiles::LaserBeam> &laser);
  GameObjects::Ships::PlayerShip *m_playerShip;

signals:
  void objectDeleted(const GameObjects::GameObject *object);
  void objectAdded(const GameObjects::GameObject *object);

public slots:
  void onObjectCreated(GameObjects::GameObject *object) {
      this->addGameObject(object);
  }
  void onStellarTokenCollected() {
    m_stellarTokens++;
  }
};
} // namespace Game

#endif // GAMESTATE_H
