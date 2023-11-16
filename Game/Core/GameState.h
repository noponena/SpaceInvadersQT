#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Projectiles/PlayerLaserBeam.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Weapons/WeaponBuilder.h"

namespace Game {
namespace Core {

class GameState : public QObject {
  Q_OBJECT
public:
  GameState(QObject *parent = nullptr);

  void initialize();
  void addGameObject(std::shared_ptr<GameObjects::GameObject> object);
  void setSize(int width, int height);
  void update(float deltaTimeInSeconds);
  void initEnemyShips();

  const std::vector<std::shared_ptr<GameObjects::GameObject>> &
  gameObjects() const;
  GameObjects::Ships::PlayerShip *playerShip() const;

  int m_minX, m_minY, m_maxX, m_maxY, m_windowWidth, m_windowHeight;

  const unsigned &stellarTokens() const;

private:
  std::vector<std::shared_ptr<GameObjects::GameObject>> m_gameObjects;
  bool m_playerShipDeletedFromScene;
  unsigned m_stellarTokens;
  float m_playersShipStartSpeed;
  void initPlayerShip();
  void initMovementConstrains();
  void addLaser(
      const std::shared_ptr<GameObjects::Projectiles::PlayerLaserBeam> &laser);
  GameObjects::Ships::PlayerShip *m_playerShip;
  Weapons::WeaponBuilder m_weaponBuilder;

signals:
  void objectDeleted(QGraphicsItem *object);
  void objectAdded(QGraphicsItem *object);
  void playerShipDestroyed();

private slots:
  void onObjectCreated(std::shared_ptr<GameObjects::GameObject> object) {
    this->addGameObject(std::move(object));
  }
  void onStellarTokenCollected() { m_stellarTokens++; }
  void onPlayerShipDestroyed() {
    m_playerShip = nullptr;
    emit playerShipDestroyed();
  }
};
} // namespace Core
} // namespace Game

#endif // GAMESTATE_H
