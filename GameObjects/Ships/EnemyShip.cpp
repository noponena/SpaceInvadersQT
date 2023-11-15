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
    : Ship(maxHp, position) {
  m_objectType = ObjectType::ENEMY_SHIP;
  m_pixmapResourcePath = ":/Images/alien.png";
  m_onHitPixmapResourcePath = ":/Images/alien_on_hit.png";
  m_pixmapScale = QPointF(50.0, 75.0);
  m_destructionSoundInfo = Game::Audio::SoundInfo(
      {m_soundEnabled, Game::Audio::SoundEffect::LESSER_ENEMY_DESTROYED});
}

void EnemyShip::initiateDestructionProcedure() {
  GameObject::initiateDestructionProcedure();
  int count = QRandomGenerator::global()->bounded(2, 5);
  this->spawnCollectables(count);
}

void EnemyShip::spawnCollectables(int amount) {
  QPointF center = this->getBoundingBox().center();
  GameObjects::Position position(center.x(), center.y());
  position.setBounds(this->getPosition().getBounds());
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
  this->shoot();
}

void EnemyShip::collideWith(GameObject &other) {
  other.collideWithEnemyShip(*this);
}

void EnemyShip::collideWithProjectile(Projectiles::Projectile &projectile) {
  this->takeDamage(projectile.getDamage());
  if (!this->isDead())
    this->playOnHitAnimation();
}

void EnemyShip::collideWithEnemyShip(EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  this->takeDamage(0);
}

QPixmap EnemyShip::getPixmap() const {
  static QPixmap pixmap = GameObject::getPixmap();
  return pixmap;
}

QPixmap EnemyShip::getOnHitPixmap() const {
  static QPixmap pixmap = GameObject::getOnHitPixmap();
  return pixmap;
}

} // namespace Ships

} // namespace GameObjects
