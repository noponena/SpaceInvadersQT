#include "PlayerLaserProjectile.h"
#include "Game/Audio/SoundInfo.h"
#include "Graphics/PixmapLibrary.h"
#include "Graphics/PixmapRegistry.h"
#include <QPen>

namespace GameObjects {
namespace Projectiles {
PlayerLaserProjectile::PlayerLaserProjectile(
    int damage, std::unordered_set<ProjectileProperty> properties)
    : Projectile(damage, properties) {
  m_pixmapData.pixmapResourcePath = ":/Images/player_laser_projectile.png";
  m_pixmapData.pixmapScale = QPointF(30, 30);
}

void PlayerLaserProjectile::registerPixmaps() {
  Graphics::PixmapLibrary::getPixmap(":/Images/player_laser_projectile.png", 30,
                                     30);
}

std::unique_ptr<Projectile> PlayerLaserProjectile::clone() const {
  std::unique_ptr<PlayerLaserProjectile> laserBeam =
      std::make_unique<PlayerLaserProjectile>();
  laserBeam->setDamage(m_damage);
  laserBeam->setProperties(m_properties);
  laserBeam->setMovementStrategy(movementStrategy());
  return laserBeam;
}

void PlayerLaserProjectile::initializeObjectType() {
  Projectile::initializeObjectType();
  m_objectTypes.insert(ObjectType::PLAYER_PROJECTILE);
}

void PlayerLaserProjectile::initializeSounds() {
  m_spawnSoundInfo =
      Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::LASER});
}

namespace {
struct PixmapRegistrar {
  PixmapRegistrar() {
    PixmapRegistry::instance().add(&PlayerLaserProjectile::registerPixmaps);
  }
};
static PixmapRegistrar _playerlaserprojectile_pixmap_registrar;
} // namespace

} // namespace Projectiles
} // namespace GameObjects
