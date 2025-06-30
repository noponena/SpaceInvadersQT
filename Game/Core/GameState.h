#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObjects/GameObject.h"
#include "GameObjects/GameObjectBuilder.h"
#include "GameObjects/Ships/PlayerShip.h"
#include "Weapons/WeaponBuilder.h"

namespace Game {
namespace Core {

class GameState : public QObject {
  Q_OBJECT
public:
  GameState(Config::GameContext gameCtx, QObject *parent = nullptr);

  void createPlayerShip();
  void initialize();
  void deinitialize();
  void addGameObject(std::shared_ptr<GameObjects::GameObject> object);
  void update(float deltaTimeInSeconds);

  const std::vector<std::shared_ptr<GameObjects::GameObject>> &
  gameObjects() const;
  std::shared_ptr<GameObjects::Ships::PlayerShip> playerShip() const;

  const std::uint32_t &stellarTokens() const;
  int enemyShipsReachedBottomLimit() const;

  int enemyShipCount() const;

private:
  Config::GameContext m_gameCtx;
  std::vector<std::shared_ptr<GameObjects::GameObject>> m_gameObjects;
  std::unordered_map<std::uint64_t, std::shared_ptr<GameObjects::GameObject>>
      m_magneticGameObjectMap;
  bool m_playerShipDeletedFromScene;
  std::uint32_t m_stellarTokens;
  float m_playersShipStartSpeed;
  int m_enemyShipsReachedBottomLimit;
  int m_enemyShipCount;
  void initPlayerShip();
  std::shared_ptr<GameObjects::Ships::PlayerShip> m_playerShip;
  Weapons::WeaponBuilder m_weaponBuilder;
  GameObjects::GameObjectBuilder m_gameObjectBuilder;

signals:
  void objectDeleted(QGraphicsItem *object);
  void objectAdded(QGraphicsItem *object);
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
