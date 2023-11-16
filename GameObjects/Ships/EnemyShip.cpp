#include "EnemyShip.h"
#include "Game/Audio/SoundInfo.h"
#include "GameObjects/Collectables/Stellar.h"
#include <QColor>
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>
#include <QTimerEvent>

namespace GameObjects {
namespace Ships {
EnemyShip::EnemyShip(const int maxHp, const Position &position)
    : ShipWithHealthBar(maxHp, 0, position) {}

void EnemyShip::executeDestructionProcedure() {
  GameObject::executeDestructionProcedure();
  int count = QRandomGenerator::global()->bounded(2, 5);
  spawnCollectables(count);
}

void EnemyShip::spawnCollectables(int amount) {
  QPointF center = getBoundingBox().center();
  GameObjects::Position position(center.x(), center.y());
  position.setBounds(getPosition().getBounds());
  for (int i = 0; i < amount; i++) {
    std::unique_ptr<GameObjects::Collectables::Stellar> stellar =
        std::make_unique<GameObjects::Collectables::Stellar>(position);
    stellar->initialize();
    emit objectCreated(std::move(stellar));
  }
}

void EnemyShip::update(const UpdateContext &context) {
  GameObject::update(context);
  m_destructionAnimation.showNextFrame();
  shoot();
}

void EnemyShip::initializeObjectType() {
  m_objectType = ObjectType::ENEMY_SHIP;
}

void EnemyShip::initializeGraphics() {
  m_pixmapResourcePath = ":/Images/alien.png";
  m_onHitPixmapResourcePath = ":/Images/alien_on_hit.png";
  m_pixmapScale = QPointF(50.0, 75.0);
}

void EnemyShip::initializeSounds() {
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_DESTROYED});
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
