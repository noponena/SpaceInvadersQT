#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Game/Audio/SoundInfo.h"
#include "Game/Movement/MovementStrategy.h"
#include "Graphics/Animations/AnimatedItem.h"
#include "Graphics/Effects/ParticleSystem.h"
#include "Position.h"
#include <QGraphicsItem>
#include <QObject>
#include <QUrl>
#include <stdexcept>
#include <unordered_set>

namespace GameObjects {

enum class ObjectType {
  UNDEFINED,
  PLAYER_SHIP,
  ENEMY_SHIP,
  PLAYER_PROJECTILE,
  ENEMY_PROJECTILE,
  STELLAR_COIN,
  HEALTH
};

namespace Projectiles {
class Projectile;
}
namespace Ships {
class EnemyShip;
class PlayerShip;
} // namespace Ships
namespace Collectables {
class Collectable;
}

struct UpdateContext {
  float deltaTimeInSeconds;
  const Ships::PlayerShip *playerShip;
};

class GameObject : public QObject {
  Q_OBJECT

public:
  // Constructors & Destructor
  GameObject(const Position &position);
  virtual ~GameObject() = default;

  // Initialization & Lifecycle
  virtual void initialize();
  virtual bool shouldBeDeleted();

  // Collision Handlers
  virtual void collideWith(GameObject &other) { Q_UNUSED(other); }
  virtual void collideWithProjectile(Projectiles::Projectile &projectile) {
    Q_UNUSED(projectile);
  }
  virtual void collideWithEnemyShip(Ships::EnemyShip &enemyShip) {
    Q_UNUSED(enemyShip);
  }
  virtual void collideWithPlayerShip(Ships::PlayerShip &playerShip) {
    Q_UNUSED(playerShip);
  }
  virtual void collideWithCollectable(Collectables::Collectable &collectable) {
    Q_UNUSED(collectable);
  }

  // Update & Movement
  virtual void update(const UpdateContext &context);
  void show();
  void hide();

  // Getters & Setters
  Position getPosition() const;
  QPointF getCenterPosition() const;
  QGraphicsScene *getScene() const;
  QGraphicsPixmapItem *getGraphicsItem() const;
  QRectF getBoundingBox() const;
  void setPosition(const Position &newPosition);
  bool isCollidable() const;
  long long unsigned id();

  // Actions & Modifiers
  void moveTo(const QPointF &newPosition);
  void setMovementStrategy(
      const Game::Movement::MovementStrategy &newMovementStrategy);
  void
  addMovementStrategy(Game::Movement::MovementStrategy &newMovementStrategy);
  void collide(GameObject &other);
  bool isCollidingWith(const GameObject &other) const;

  void setPosition(const QPointF &newPosition);
  void setSoundEnabled(const bool newSoundEnabled);

  ObjectType objectType() const;
  virtual bool isDead() { return false; };

  Game::Movement::MovementStrategy movementStrategy() const;

protected:
  // Member Variables
  ObjectType m_objectType;
  Position m_position;
  std::unique_ptr<QGraphicsPixmapItem> m_graphicsItem;
  bool m_hasCollided;
  bool m_collidable;
  bool m_soundEnabled;
  std::unordered_set<int> m_collisions;
  QPointF m_pixmapScale;
  QString m_pixmapResourcePath;
  QString m_onHitPixmapResourcePath;

  Graphics::Effects::ParticleSystem m_destructionEffect;
  Graphics::Animations::AnimatedItem m_destructionAnimation;

  struct Game::Audio::SoundInfo m_spawnSoundInfo;
  struct Game::Audio::SoundInfo m_destructionSoundInfo;

  // Protected Helpers & Methods
  virtual void initializeObjectType() = 0;
  virtual void initializeGraphics() = 0;
  virtual void initializeSounds() = 0;

  virtual void initializeDestructionAnimation(){};
  virtual void initializeDestructionEffects(){};

  virtual void playDestructionAnimation();
  virtual void playDestructionEffects();
  virtual void executeDestructionProcedure();
  virtual void disableMovement();

  QPixmap getPixmap() const;
  QPixmap getOnHitPixmap() const;

  // Protected Helpers & Methods
  void clampToXBounds();
  void clampToYBounds();

private:
  // Member Variables
  long long unsigned m_id;
  bool m_destructionInitiated;
  Game::Movement::MovementStrategy m_movementStrategy;
  static long long unsigned counter;

  inline void applyMovementStrategy(float deltaTimeInSeconds);
  inline void playSpawnSound();
  inline void playDestructionSound();
  inline void updateGraphicsItemPosition();

signals:
  void objectCreated(std::shared_ptr<GameObjects::GameObject> object);
  void objectDestroyed();
};

} // namespace GameObjects

#endif // GAMEOBJECT_H
