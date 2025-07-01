#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Config/GameContext.h"
#include "Game/Audio/SoundInfo.h"
#include "Game/Movement/MovementStrategy.h"
#include "Graphics/Animations/AnimationPlayer.h"
#include "Utils/BoundsChecker.h"
#include <QGraphicsItem>
#include <QObject>
#include <QUrl>
#include <qvectornd.h>
#include <unordered_set>

namespace GameObjects {
class GameObject;

enum class ObjectType {
  BASE,

  SHIP,
  PLAYER_SHIP,
  ENEMY_SHIP,

  PROJECTILE,
  PLAYER_PROJECTILE,
  ENEMY_PROJECTILE,

  COLLECTABLE,
  STELLAR_COIN,
  HEALTH
};

enum class ConcreteType {
  PLAYER_SHIP,
  ENEMY_SHIP,
  PROJECTILE,
  VORTEX,
  WAVE_OF_DESTRUCTION,
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

struct Magnetism {
  bool isMagnetic = false;
  bool enabled = false;
  float magneticRadius = 0.0f;
  float magneticStrength = 0.0f;
};

struct UpdateContext {
  float deltaTimeInSeconds;
  const Ships::PlayerShip *playerShip;
  const std::unordered_map<std::uint64_t,
                           std::shared_ptr<GameObjects::GameObject>>
      &magneticGameObjects;
};

enum class State {
  Normal,
  OnHit,
  OnDestruction,
};

struct RenderData {
  QVector2D size;    // Floating-point width/height
  QString imagePath; // Path or key to look up the texture
  float rotation;    // Radians

  QVector2D uvMin = QVector2D(0.0f, 0.0f); // lower-left
  QVector2D uvMax = QVector2D(1.0f, 1.0f); // upper-right

  RenderData(const QVector2D &sz = QVector2D(10.0f, 10.0f),
             const QString &path = QString(":/Images/placeholder.png"),
             float rot = 0.0f)
      : size(sz), imagePath(path), rotation(rot) {}
};

struct Transform {
  QVector2D position;
  QVector2D anchorPosition;
  QVector2D colliderSize; // Width/height of the collision box
  float rotation;

  Transform(const QVector2D &pos = QVector2D(0.0f, 0.0f),
            const QVector2D &anchor = QVector2D(0.0f, 0.0f),
            const QVector2D &csize = QVector2D(10.0f, 10.0f), float rot = 0.0f)
      : position(pos), anchorPosition(anchor), colliderSize(csize),
        rotation(rot) {}

  QRectF colliderRect() const {
    float left = position.x() - colliderSize.x() * 0.5f;
    float top = position.y() - colliderSize.y() * 0.5f;
    return QRectF(left, top, colliderSize.x(), colliderSize.y());
  }
};

using RenderDataMap = std::unordered_map<State, RenderData>;
using AnimationDataMap =
    std::unordered_map<State, Graphics::Animations::AnimationInfo>;

class GameObject : public QObject {
  Q_OBJECT

public:
  // Constructors & Destructor
  GameObject(const Config::GameContext &ctx);
  virtual ~GameObject() = default;
  virtual std::unique_ptr<GameObject> clone() const = 0;

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
  const QVector2D &getPosition() const;
  QPointF getCenterPosition() const;
  QRectF getBoundingBox() const;
  QString hudPixmapPath() const;
  ;
  bool isCollidable() const;
  bool isVisible() const;
  std::uint64_t id() const;
  const Magnetism &magnetism() const;

  // Actions & Modifiers
  void setMovementStrategy(
      const Game::Movement::MovementStrategy &newMovementStrategy);
  void
  addMovementStrategy(Game::Movement::MovementStrategy &newMovementStrategy);
  void collide(GameObject &other);
  bool isCollidingWith(const GameObject &other) const;

  void moveAbsolute(const QVector2D &newPosition);
  void moveRelative(const QVector2D &displacement);
  void setSoundEnabled(const bool newSoundEnabled);
  void addObjectType(const ObjectType objectType);

  std::unordered_set<ObjectType> objectTypes() const;
  virtual bool isDead() { return false; };

  Game::Movement::MovementStrategy movementStrategy() const;

  void setSpawnSoundInfo(const Game::Audio::SoundInfo &newSpawnSoundInfo);
  void setDestructionSoundInfo(
      const Game::Audio::SoundInfo &newDestructionSoundInfo);

  RenderData renderData() const;

  void setState(State newState);
  State state() const;

  const RenderData &getRenderData() const;

  void addRenderData(State state, const RenderData &data);

  Transform transform() const;

  const RenderData &getRenderData(State state) const;

  RenderDataMap renderDataByState() const;

  void setRenderDataByState(const RenderDataMap &newRenderDataByState);

  void setTransform(const Transform &newTransform);

  void setGameContext(const Config::GameContext &newGameContext);

  Config::GameContext gameContext() const;

protected:
  // Member Variables
  std::unordered_set<ObjectType> m_objectTypes;
  Transform m_transform;
  RenderDataMap m_renderDataByState;
  AnimationDataMap m_animationInfoByState;
  State m_state = State::Normal;
  bool m_hasCollided;
  bool m_collidable;
  bool m_soundEnabled;
  bool m_hasDestructionEffect;
  std::unordered_set<int> m_collisions;
  Magnetism m_magnetism;
  Config::GameContext m_gameContext;

  Graphics::Animations::AnimationPlayer m_animationPlayer;

  Game::Audio::SoundInfo m_spawnSoundInfo;
  Game::Audio::SoundInfo m_destructionSoundInfo;

  // Protected Helpers & Methods
  virtual void initializeObjectType() = 0;
  virtual void initializeSounds() = 0;

  virtual void initializeDestructionAnimation() {};
  virtual void initializeDestructionEffects() {};

  virtual void playDestructionAnimation();
  virtual void playDestructionEffects();
  virtual void executeDestructionProcedure();
  virtual void disableMovement();

  // Protected Helpers & Methods
  void clampToXBounds();
  void clampToYBounds();

  bool hasDestructionAnimation() const;

private:
  // Member Variables
  std::uint32_t m_id;
  bool m_visible = true;
  bool m_destructionInitiated;
  Game::Movement::MovementStrategy m_movementStrategy;
  static std::uint32_t counter;

  inline void applyMovementStrategy(float deltaTimeInSeconds);
  inline void playSpawnSound();
  inline void playDestructionSound();

  void
  setFrameUvsForCurrentState(const std::pair<QVector2D, QVector2D> frameUvs);

signals:
  void objectCreated(std::shared_ptr<GameObjects::GameObject> object);
  void objectDestroyed();
};

} // namespace GameObjects

#endif // GAMEOBJECT_H
