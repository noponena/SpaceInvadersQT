#include "ProjectileBuilder.h"

namespace GameObjects {
namespace Projectiles {

ProjectileBuilder::ProjectileBuilder() {}

ProjectileBuilder ProjectileBuilder::clone() const {
  ProjectileBuilder builder;
  if (m_projectile)
    builder.m_projectile = std::unique_ptr<Projectile>(
        static_cast<Projectile *>(this->m_projectile->clone().release()));
  else
    logNullPointerWarning();
  return builder;
}

ProjectileBuilder &
ProjectileBuilder::withObjectType(const ObjectType objectType) {
  if (m_projectile)
    m_projectile->addObjectType(objectType);
  return *this;
}

ProjectileBuilder &ProjectileBuilder::withDamage(const int damage) {
  if (m_projectile)
    m_projectile->setDamage(damage);
  else
    logNullPointerWarning();
  return *this;
}

ProjectileBuilder &ProjectileBuilder::withMovementStrategy(
    const Game::Movement::MovementStrategy &strategy) {
  if (m_projectile)
    m_projectile->setMovementStrategy(strategy);
  else
    logNullPointerWarning();
  return *this;
}

ProjectileBuilder &
ProjectileBuilder::withProperty(const ProjectileProperty property) {
  if (m_projectile)
    m_projectile->addProperty(property);
  else
    logNullPointerWarning();
  return *this;
}

ProjectileBuilder &
ProjectileBuilder::withSpawnSound(const Game::Audio::SoundInfo spawnSound) {
  if (m_projectile)
    m_projectile->setSpawnSoundInfo(spawnSound);
  return *this;
}

ProjectileBuilder &ProjectileBuilder::withDestructionSound(
    const Game::Audio::SoundInfo destructionSound) {
  if (m_projectile)
    m_projectile->setDestructionSoundInfo(destructionSound);
  return *this;
}

ProjectileBuilder &ProjectileBuilder::withGrahpics(const RenderDataMap renderDataMap)
{
    if (m_projectile)
        m_projectile->setRenderDataByState(renderDataMap);
    return *this;
}

std::unique_ptr<Projectile> ProjectileBuilder::build() {
  return std::unique_ptr<Projectile>(
      static_cast<Projectile *>(this->m_projectile->clone().release()));
}

void ProjectileBuilder::logNullPointerWarning() const {
  qCritical() << "createProjectile must be called before setting any weapon "
                 "properties.";
}
} // namespace Projectiles
} // namespace GameObjects
