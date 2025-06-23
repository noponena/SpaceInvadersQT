#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Weapons/WeaponBuilder.h"

namespace Game {
namespace Core {

class GameState : public QObject {
  Q_OBJECT
public:
  GameState(QObject *parent = nullptr);

  void createPlayerShip();
  void initialize();
  void deinitialize();
  void addGameObject(std::shared_ptr<GameObjects::GameObject> object);
  void setSize(int width, int height);
  void update(float deltaTimeInSeconds);

  const std::vector<std::shared_ptr<GameObjects::GameObject>> &
  gameObjects() const;
  GameObjects::Ships::PlayerShip *playerShip() const;

  int m_minX, m_minY, m_maxX, m_maxY, m_windowWidth, m_windowHeight;

  const std::uint32_t &stellarTokens () const;
  int enemyShipsReachedBottomLimit() const;

  int enemyShipCount() const;

private:
  std::vector<std::shared_ptr<GameObjects::GameObject>> m_gameObjects;
  std::unordered_map<std::uint64_t, std::shared_ptr<GameObjects::GameObject>>
      m_magneticGameObjectMap;
  bool m_playerShipDeletedFromScene;
  std::uint32_t m_stellarTokens;
  float m_playersShipStartSpeed;
  int m_enemyShipsReachedBottomLimit;
  int m_enemyShipCount;
  void initPlayerShip();
  void initMovementConstrains();
  std::shared_ptr<GameObjects::Ships::PlayerShip> m_playerShip;
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder m_projectileBuilder;

signals:
  void objectDeleted(QGraphicsItem *object);
  void objectAdded(QGraphicsItem *object);
  void gameObjectAdded(GameObjects::GameObject *object);
  void gameObjectRemoved(GameObjects::GameObject *object);
  void playerShipDestroyed();

private slots:
  void onObjectCreated(std::shared_ptr<GameObjects::GameObject> object) {
    addGameObject(std::move(object));
  }
  void onStellarTokenCollected() { m_stellarTokens++; }
  void onEnemyReachedBottomEdge() { m_enemyShipsReachedBottomLimit++; };
  void onEnemyShipDeleted() { m_enemyShipCount--; }
  void onPlayerShipDestroyed() {
    m_playerShip = nullptr;
    emit playerShipDestroyed();
  }
};
} // namespace Core
} // namespace Game

#endif // GAMESTATE_H
