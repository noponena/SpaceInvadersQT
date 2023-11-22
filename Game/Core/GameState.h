#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Weapons/WeaponBuilder.h"
#include <mutex>

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
  std::mutex &mutex();

private:
  mutable std::mutex m_mutex;
  std::vector<std::pair<uint64_t, uint64_t>> m_collidingPairs;
  std::vector<std::shared_ptr<GameObjects::GameObject>> m_gameObjects;
  std::unordered_map<uint64_t, std::shared_ptr<GameObjects::GameObject>> m_magneticGameObjectMap;
  bool m_playerShipDeletedFromScene;
  unsigned m_stellarTokens;
  float m_playersShipStartSpeed;
  void initPlayerShip();
  void initMovementConstrains();
  GameObjects::Ships::PlayerShip *m_playerShip;
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder m_projectileBuilder;

signals:
  void objectDeleted(QGraphicsItem *object);
  void objectAdded(QGraphicsItem *object);
  void playerShipDestroyed();

private slots:
  void onObjectCreated(std::shared_ptr<GameObjects::GameObject> object) {
    addGameObject(std::move(object));
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
