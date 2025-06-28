#include "PerformanceBenchmark.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "Utils/Math/MathFunctions.h"
#include "Utils/Utils.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/WeaponBuilder.h"
#include <QDebug>
#include <iostream>
#include <sstream>
#ifdef _WIN32
// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on
#else
#include <sys/resource.h>
#include <unistd.h>
#endif

namespace Utils {

PerformanceBenchmark::PerformanceBenchmark(int logIntervalMs,
                                           int gameObjectThreshold,
                                           char csvDelimiter,
                                           const Config::GameContext ctx)
    : m_gameCtx(ctx), m_logIntervalMs(logIntervalMs),
      m_gameObjectThreshold(gameObjectThreshold), m_csvDelimiter(csvDelimiter) {

  std::filesystem::path exePath = std::filesystem::current_path();
  std::filesystem::path performanceDir = exePath / "performance";
  std::filesystem::create_directory(performanceDir);

  m_filePath = performanceDir / "benchmark_scores.csv";

  // Check if the file exists, and if not, create it
  if (!std::filesystem::exists(m_filePath)) {
    // Create and close the file
    std::ofstream ofs(m_filePath);
    openFile();
    writeHeader();
    closeFile();
  }
}

std::vector<float>
PerformanceBenchmark::filteredFrameTimes(float maxAllowedMs,
                                         size_t skipFirstN) const {
  std::vector<float> filtered;
  filtered.reserve(m_frameTimesMs.size() > skipFirstN
                       ? m_frameTimesMs.size() - skipFirstN
                       : 0);
  for (size_t i = skipFirstN; i < m_frameTimesMs.size(); ++i) {
    float ft = m_frameTimesMs[i];
    if (ft > 0.0f && ft <= maxAllowedMs) {
      filtered.push_back(ft);
    }
  }
  return filtered;
}

PerformanceBenchmark::~PerformanceBenchmark() {
  if (m_outFile.is_open()) {
    closeFile();
  }
}

void PerformanceBenchmark::initializeBenchmark(
    std::shared_ptr<GameObjects::Ships::PlayerShip> playerShip) {
  Game::Movement::MovementStrategy horizontalStrategyLeft =
      Game::Movement::HorizontalMovementStrategy(240, 1);
  Game::Movement::MovementStrategy horizontalStrategyRight =
      Game::Movement::HorizontalMovementStrategy(240, -1);
  Game::Movement::MovementStrategy horizontalCombined =
      horizontalStrategyLeft + horizontalStrategyRight;

  Game::Movement::IntervalMovementStrategy horizontalIntervalStrategy =
      Game::Movement::IntervalMovementStrategy(horizontalCombined, 12.5f);

  Weapons::WeaponBuilder weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder projectileBuilder;

  GameObjects::RenderDataMap renderDataMap{
      {GameObjects::State::Normal,
       GameObjects::RenderData({30, 30},
                               ":/Images/player_laser_projectile.png")}};

  std::unique_ptr<GameObjects::Projectiles::Projectile> primaryProjectile =
      projectileBuilder
          .createProjectile<GameObjects::Projectiles::Projectile>(m_gameCtx)
          .withDamage(999)
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withGrahpics(renderDataMap)
          .withSpawnSound(
              Game::Audio::SoundInfo({true, Game::Audio::SoundEffect::LASER}))
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(1000, -1))
          .build();

  // Create the primary weapon using WeaponBuilder
  std::unique_ptr<Weapons::Weapon> weapon =
      weaponBuilder.createWeapon<Weapons::PrimaryWeapon>()
          .withProjectile(std::move(primaryProjectile))
          .withWeaponCooldownMs(0)
          .build();

  // Clone the primary weapon and modify the projectile for the second weapon
  std::unique_ptr<Weapons::Weapon> secondWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, 10))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the third weapon
  std::unique_ptr<Weapons::Weapon> thirdWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, -10))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the fourth weapon
  std::unique_ptr<Weapons::Weapon> fourthWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, -5))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the fifth weapon
  std::unique_ptr<Weapons::Weapon> fifthWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, 5))
                  .build())
          .build();

  playerShip->clearWeapons();
  playerShip->addPrimaryWeapon(std::move(weapon));
  playerShip->addPrimaryWeapon(std::move(secondWeapon));
  playerShip->addPrimaryWeapon(std::move(thirdWeapon));
  playerShip->addPrimaryWeapon(std::move(fourthWeapon));
  playerShip->addPrimaryWeapon(std::move(fifthWeapon));

  QVector2D position = playerShip->getPosition();
  position.setX(0.05);
  playerShip->moveAbsolute(position);
  playerShip->setMovementStrategy(horizontalIntervalStrategy);
  playerShip->updateFireRate(-99999);
  playerShip->setImmortal(true);
  playerShip->setAutoShoot(true);
}

float PerformanceBenchmark::getMemUsage() {
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc,
                       sizeof(pmc));
  SIZE_T privateMemUsedByMe = pmc.PrivateUsage;
  return static_cast<float>(privateMemUsedByMe) / (1024 * 1024);
#else
  // Linux/macOS: get resident set size (RSS) in MB
  struct rusage usage;
  if (getrusage(RUSAGE_SELF, &usage) == 0) {
    // ru_maxrss is in kilobytes on Linux, bytes on macOS. We handle Linux
    // here:
    return static_cast<float>(usage.ru_maxrss) / 1024.0f;
  }
  return 0.0f;
#endif
}

void PerformanceBenchmark::createFile() {
  openFile();
  if (m_outFile.is_open()) {
    writeHeader();
  }
}

void PerformanceBenchmark::openFile() {
  m_outFile.open(m_filePath, std::ios::out | std::ios::app);
}

void PerformanceBenchmark::closeFile() { m_outFile.close(); }

void PerformanceBenchmark::writeHeader() {
  m_outFile << "timestamp" << m_csvDelimiter << "avg_fps" << m_csvDelimiter
            << "min_fps" << m_csvDelimiter << "p95_fps" << m_csvDelimiter
            << "p99_fps" << m_csvDelimiter << "mem_usage\n";
}

void PerformanceBenchmark::recordFrameTime(int frameTimeMs) {
  m_frameTimesMs.push_back(static_cast<float>(frameTimeMs));
}

void PerformanceBenchmark::logPerformanceScore() {
  auto filtered = filteredFrameTimes();

  // Calculate FPS metrics
  float avgFrameTime = 0.0f;
  float maxFrameTime = 0.0f;
  float p95FrameTime = 0.0f;
  float p99FrameTime = 0.0f;

  if (!filtered.empty()) {
    float sum = 0.0f;
    maxFrameTime = filtered[0];
    for (float ft : filtered) {
      sum += ft;
      if (ft > maxFrameTime)
        maxFrameTime = ft;
    }
    avgFrameTime = sum / filtered.size();
    p95FrameTime = MathFunctions::percentile(filtered, 0.95f);
    p99FrameTime = MathFunctions::percentile(filtered, 0.99f);
  } else {
    avgFrameTime = 0.0f;
    maxFrameTime = 0.0f;
    p95FrameTime = 0.0f;
    p99FrameTime = 0.0f;
  }

  float avgFps = avgFrameTime > 0.0f ? 1000.0f / avgFrameTime : 0.0f;
  float minFps = maxFrameTime > 0.0f ? 1000.0f / maxFrameTime : 0.0f;
  float p95Fps = p95FrameTime > 0.0f ? 1000.0f / p95FrameTime : 0.0f;
  float p99Fps = p99FrameTime > 0.0f ? 1000.0f / p99FrameTime : 0.0f;

  float memUsage = getMemUsage();

  std::stringstream ss;
  ss << Utils::getTimestampStr();
  std::ofstream file(m_filePath, std::ios::app);
  if (file.is_open()) {
    file << ss.str() << m_csvDelimiter << avgFps << m_csvDelimiter << minFps
         << m_csvDelimiter << p95Fps << m_csvDelimiter << p99Fps
         << m_csvDelimiter << memUsage << std::endl;
  }
  m_frameTimesMs.clear();
}

} // namespace Utils
