#include "EnemyShip.h"
#include "Game/Audio/SoundInfo.h"
#include "GameObjects/Collectables/Health.h"
#include "GameObjects/Collectables/Stellar.h"
#include "Utils/Utils.h"
#include <QColor>
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {
EnemyShip::EnemyShip(const Config::GameContext &ctx)
    : ShipWithHealthBar(ctx), m_bottomEdgeSignalEmitted(false) {
  m_stellarCoinSpawnRange = QPoint(2, 5);
  m_healthSpawnProbability = 0.10;

  RenderData normalData;
  normalData.size = QVector2D(50, 50);
  normalData.imagePath = ":/Images/alien.png";
  addRenderData(State::Normal, normalData);

  RenderData onHitData;
  onHitData.size = QVector2D(50, 50);
  onHitData.imagePath = ":/Images/alien_on_hit.png";
  addRenderData(State::OnHit, onHitData);

  RenderData onDestructionData;
  onHitData.size = QVector2D(50, 50);
  onHitData.imagePath = ":/Images/explosion.png";
  addRenderData(State::OnDestruction, onDestructionData);

  m_transform.colliderSize = {30, 30};

  m_magneticTargets = {ObjectType::PROJECTILE};
}

void EnemyShip::initializeObjectType() {
  Ship::initializeObjectType();
  m_objectTypes.insert(ObjectType::ENEMY_SHIP);
}

void EnemyShip::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_DESTROYED});
}

void EnemyShip::executeDestructionProcedure() {
  GameObject::executeDestructionProcedure();
  spawnStellarCoins();
  spawnHealth();
}

void EnemyShip::spawnStellarCoins() {
  int amount = QRandomGenerator::global()->bounded(m_stellarCoinSpawnRange.x(),
                                                   m_stellarCoinSpawnRange.y());
  QPointF center = getBoundingBox().center();
  QVector2D position(center.x(), center.y());
  for (int i = 0; i < amount; i++) {
    std::unique_ptr<GameObjects::Collectables::Stellar> stellar =
        std::make_unique<GameObjects::Collectables::Stellar>(m_gameContext);
    stellar->moveAbsolute(position);
    stellar->initialize();
    emit objectCreated(std::move(stellar));
  }
}

void EnemyShip::spawnHealth() {
  if (Utils::probabilityCheck(m_healthSpawnProbability)) {
    QPointF center = getBoundingBox().center();
    QVector2D position(center.x(), center.y());

    std::unique_ptr<GameObjects::Collectables::Health> health =
        std::make_unique<GameObjects::Collectables::Health>(m_gameContext);
    health->moveAbsolute(position);
    health->initialize();
    emit objectCreated(std::move(health));
  }
}

void EnemyShip::setHealthSpawnProbability(float newHealthSpawnProbability) {
  m_healthSpawnProbability = newHealthSpawnProbability;
  clampHealthSpawnProbability();
}

void EnemyShip::updateHealthSpawnProbability(float multiplier) {
  m_healthSpawnProbability *= multiplier;
  clampHealthSpawnProbability();
}

void EnemyShip::setStellarCoinSpawnRange(QPoint newStellarCoinSpawnRange) {
  m_stellarCoinSpawnRange = newStellarCoinSpawnRange;
}

void EnemyShip::clampHealthSpawnProbability() {
  if (m_healthSpawnProbability < 0)
    m_healthSpawnProbability = 0;
  else if (m_healthSpawnProbability > 1)
    m_healthSpawnProbability = 1;
}

std::unique_ptr<GameObject> EnemyShip::clone() const {
  std::unique_ptr<EnemyShip> enemyShip =
      std::make_unique<EnemyShip>(m_gameContext);

  enemyShip->setTransform(m_transform);
  enemyShip->setMaxHealth(m_maxHealth);
  enemyShip->m_destructionSoundInfo = m_destructionSoundInfo;
  enemyShip->m_objectTypes = m_objectTypes;
  enemyShip->m_healthSpawnProbability = m_healthSpawnProbability;
  enemyShip->m_stellarCoinSpawnRange = m_stellarCoinSpawnRange;
  enemyShip->m_magneticTargets = m_magneticTargets;
  enemyShip->m_fireCooldownMs = m_fireCooldownMs;
  enemyShip->setMovementStrategy(movementStrategy());

  enemyShip->m_energyRegenerationRate = m_energyRegenerationRate;
  enemyShip->m_currentEnergy = m_currentEnergy;
  enemyShip->m_currentHealth = m_currentHealth;
  enemyShip->m_maxEnergy = m_maxEnergy;
  enemyShip->m_originalColor = m_originalColor;
  enemyShip->m_autoFire = m_autoFire;

  for (auto &weapon : m_primaryWeapons) {
    enemyShip->addPrimaryWeapon(weapon->clone());
  }

  enemyShip->initialize();
  return enemyShip;
}

bool EnemyShip::shouldBeDeleted() {
  if (Utils::BoundsChecker::isBeyondScreenBottom(
          m_transform.position, m_gameContext.movementBounds) &&
      !m_bottomEdgeSignalEmitted) {
    emit bottomEdgeReached();
    m_bottomEdgeSignalEmitted = true;
  }

  bool shouldBeDeleted = ShipWithHealthBar::shouldBeDeleted();
  if (shouldBeDeleted)
    emit enemyShipDeleted();
  return shouldBeDeleted;
}

void EnemyShip::collideWith(GameObject &other) {
  other.collideWithEnemyShip(*this);
}

void EnemyShip::collideWithProjectile(Projectiles::Projectile &projectile) {
  takeDamage(projectile.getDamage());
  if (!isDead())
    playOnHitAnimation();
}

void EnemyShip::collideWithEnemyShip(EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  takeDamage(0);
}

} // namespace Ships
} // namespace GameObjects
