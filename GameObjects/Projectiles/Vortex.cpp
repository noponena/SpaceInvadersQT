#include "Vortex.h"
#include "Graphics/PixmapLibrary.h"
#include "Graphics/PixmapRegistry.h"

namespace GameObjects {
namespace Projectiles {

Vortex::Vortex() : m_timeToLiveSeconds(5.0f), m_timeSinceSpawnSeconds(0.0f) {
  m_magnetism = Magnetism{true, false, 200, 200};
  m_spawnSoundInfo =
      Game::Audio::SoundInfo({true, Game::Audio::SoundEffect::VORTEX_CANNON});
  m_pixmapData.pixmapResourcePath = ":/Images/black_orb.png";
  m_pixmapData.onHitPixmapResourcePath = ":/Images/black_hole.png";
  m_pixmapData.hudPixmapResourcePath = ":/Images/black_hole_hud.png";
  m_pixmapData.pixmapScale = QPointF(25, 25);
}

void Vortex::registerPixmaps() {
  Graphics::PixmapLibrary::getPixmap(":/Images/black_orb.png", 25, 25);
  Graphics::PixmapLibrary::getPixmap(":/Images/black_hole.png", 25, 25);
  Graphics::PixmapLibrary::getPixmap(":/Images/black_hole_hud.png", 25, 25);
}

void Vortex::update(const UpdateContext &context) {
  Projectile::update(context);
  m_timeSinceSpawnSeconds += context.deltaTimeInSeconds;
}

void Vortex::collideWithEnemyShip(Ships::EnemyShip &enemyShip) {
  Q_UNUSED(enemyShip);
  m_magnetism.enabled = true;
  m_collidable = false;
  m_pixmapData.pixmapScale = QPointF(100, 100);
  m_graphicsItem->setPixmap(getOnHitPixmap());
  disableMovement();
}

bool Vortex::shouldBeDeleted() {
  return GameObject::shouldBeDeleted() ||
         m_timeSinceSpawnSeconds >= m_timeToLiveSeconds;
}

std::unique_ptr<GameObject> Vortex::clone() const {
  std::unique_ptr<Vortex> projectile = std::make_unique<Vortex>();
  projectile->m_objectTypes = m_objectTypes;
  projectile->m_magnetism = m_magnetism;
  projectile->setSpawnSoundInfo(m_spawnSoundInfo);
  projectile->setDestructionSoundInfo(m_destructionSoundInfo);
  projectile->setDamage(m_damage);
  projectile->setProperties(m_properties);
  projectile->setPixmapData(m_pixmapData);
  projectile->setMovementStrategy(movementStrategy());
  return projectile;
}

namespace {
struct PixmapRegistrar {
  PixmapRegistrar() {
    PixmapRegistry::instance().add(&Vortex::registerPixmaps);
  }
};
static PixmapRegistrar _vortex_pixmap_registrar;
} // namespace

} // namespace Projectiles
} // namespace GameObjects
