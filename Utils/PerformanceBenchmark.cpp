#include "PerformanceBenchmark.h"
#include "GameObjects/Projectiles/ProjectileBuilder.h"
#include "Weapons/PrimaryWeapon.h"
#include "Weapons/WeaponBuilder.h"
#include <QDebug>
#include <chrono>
#include <iomanip>
#ifdef _WIN32
// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on
#else
#include <sys/resource.h>
#include <unistd.h>
#endif
#include <sstream>

namespace Utils {

PerformanceBenchmark::PerformanceBenchmark(int logIntervalMs,
                                           int gameObjectThreshold,
                                           char csvDelimiter)
    : m_logIntervalMs(logIntervalMs),
      m_gameObjectThreshold(gameObjectThreshold), m_csvDelimiter(csvDelimiter),
      m_accumulatedTimeSinceLastLogMs(0) {

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

PerformanceBenchmark::~PerformanceBenchmark() {
  if (m_outFile.is_open()) {
    closeFile();
  }
}

void PerformanceBenchmark::initializeBenchmark(
    GameObjects::Ships::PlayerShip *playerShip) {
  Game::Movement::MovementStrategy horizontalStrategyLeft =
      Game::Movement::HorizontalMovementStrategy(240, 1);
  Game::Movement::MovementStrategy horizontalStrategyRight =
      Game::Movement::HorizontalMovementStrategy(240, -1);
  Game::Movement::MovementStrategy horizontalCombined =
      horizontalStrategyLeft + horizontalStrategyRight;

  Game::Movement::IntervalMovementStrategy horizontalIntervalStrategy =
      Game::Movement::IntervalMovementStrategy(horizontalCombined, 10.0f);

  Weapons::WeaponBuilder weaponBuilder;
  GameObjects::Projectiles::ProjectileBuilder projectileBuilder;

  std::unique_ptr<GameObjects::Projectiles::Projectile> primaryProjectile =
      projectileBuilder.createProjectile<GameObjects::Projectiles::Projectile>()
          .withDamage(1)
          .withObjectType(GameObjects::ObjectType::PLAYER_PROJECTILE)
          .withGrahpics(GameObjects::PixmapData{
              QPointF(30, 30), ":/Images/player_laser_projectile.png", "", ""})
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
                      Game::Movement::AngledMovementStrategy(1000, -1, 80))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the third weapon
  std::unique_ptr<Weapons::Weapon> thirdWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, -80))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the fourth weapon
  std::unique_ptr<Weapons::Weapon> fourthWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, 1, -85))
                  .build())
          .build();

  // Clone the primary weapon and modify the projectile for the fifth weapon
  std::unique_ptr<Weapons::Weapon> fifthWeapon =
      weaponBuilder.clone()
          .withProjectile(
              projectileBuilder
                  .withMovementStrategy(
                      Game::Movement::AngledMovementStrategy(1000, -1, 85))
                  .build())
          .build();

  playerShip->clearWeapons();
  playerShip->addPrimaryWeapon(std::move(weapon));
  playerShip->addPrimaryWeapon(std::move(secondWeapon));
  playerShip->addPrimaryWeapon(std::move(thirdWeapon));
  playerShip->addPrimaryWeapon(std::move(fourthWeapon));
  playerShip->addPrimaryWeapon(std::move(fifthWeapon));

  GameObjects::Position position = playerShip->getPosition();
  position.setX(0);
  playerShip->setPosition(position);
  playerShip->setMovementStrategy(horizontalIntervalStrategy);
  playerShip->updateFireRate(-99999);
  playerShip->setImmortal(true);
  playerShip->setAutoShoot(true);
}

float PerformanceBenchmark::calculateBenchmarkScore() {
  if (m_stats.empty())
    return 0.0f;

  float totalFrameTime = 0.0;

  for (const auto &data : m_stats) {
    float frameTime = std::get<0>(data);
    totalFrameTime += frameTime;
  }

  return m_gain / (totalFrameTime / m_stats.size());
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

bool PerformanceBenchmark::isTimeToLog(int frameTimeMs) {
  m_accumulatedTimeSinceLastLogMs += frameTimeMs;
  return m_accumulatedTimeSinceLastLogMs >= m_logIntervalMs;
}

void PerformanceBenchmark::writeHeader() {
  m_outFile << "timestamp" << m_csvDelimiter << "score" << m_csvDelimiter
            << "mem_usage\n";
}

void PerformanceBenchmark::logPerformance(int frameTimeMs, int gameObjectCount,
                                          int sceneItemCount) {
  if (isTimeToLog(frameTimeMs)) {
    if (gameObjectCount >= m_gameObjectThreshold) {
      float frameTimePerGameObject =
          gameObjectCount > 0 ? static_cast<float>(frameTimeMs) /
                                    static_cast<float>(gameObjectCount)
                              : 0.0f;

      float frameTimePerSceneItem = sceneItemCount > 0
                                        ? static_cast<float>(frameTimeMs) /
                                              static_cast<float>(sceneItemCount)
                                        : 0.0f;

      m_stats.emplace_back(
          std::make_tuple(frameTimeMs, gameObjectCount, sceneItemCount,
                          frameTimePerGameObject, frameTimePerSceneItem));
    }
    m_accumulatedTimeSinceLastLogMs = 0;
  }
}

void PerformanceBenchmark::logPerformanceScore() {
  float score = calculateBenchmarkScore();

  // Get the current time
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);

  // Convert to broken-down time
  std::tm *broken_down_time = std::gmtime(&now_c);

  // Manually format the datetime
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(4) << broken_down_time->tm_year + 1900
     << '-' << std::setw(2) << broken_down_time->tm_mon + 1 << '-'
     << std::setw(2) << broken_down_time->tm_mday << 'T' << std::setw(2)
     << broken_down_time->tm_hour << ':' << std::setw(2)
     << broken_down_time->tm_min << ':' << std::setw(2)
     << broken_down_time->tm_sec << 'Z';

  float memUsage = getMemUsage();

  // Open the file in append mode
  std::ofstream file(m_filePath, std::ios::app);
  if (file.is_open()) {
    // Write the timestamp and the score to the file
    file << ss.str() << m_csvDelimiter << score << m_csvDelimiter << memUsage
         << std::endl;
  }
}

} // namespace Utils
