#include "LevelLoader.h"
#include "Weapons/PrimaryWeapon.h"
#include <QDebug>
#include <filesystem>
#include <regex>
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Levels {

LevelLoader::LevelLoader(Core::GameState *gameState, int screenWidth,
                         int screenHeight)
    : m_gameState(gameState), m_screenWidth(screenWidth),
      m_screenHeight(screenHeight) {
  int minX = m_gameState->m_minX;
  int maxX = m_gameState->m_maxX;
  int minY = m_gameState->m_minY;
  int maxY = m_gameState->m_maxY;

  // 2. Create a new enemy ship
  GameObjects::Position pos(0, 0, minX, maxX, minY, maxY);

  GameObjects::PixmapData pixmapData{
      QPointF(30, 30), ":/Images/enemy_laser_projectile.png", "", ""};

  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      m_projectileBuilder
          .createProjectile(
              std::make_unique<GameObjects::Projectiles::Projectile>())
          .withDamage(1)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(500, 1))
          .withGrahpics(pixmapData)
          .withSpawnSound(Audio::SoundInfo(
              {true, Game::Audio::SoundEffect::LESSER_ENEMY_LASER}))
          .withObjectType(GameObjects::ObjectType::ENEMY_PROJECTILE)
          .build();

  m_weaponBuilder.createWeapon(std::make_unique<Weapons::PrimaryWeapon>())
      .withProjectile(std::move(projectile))
      .withWeaponCooldownMs(2500);

  m_enemyShip = std::make_unique<GameObjects::Ships::EnemyShip>(5, pos);
  m_enemyShip->initialize();

  m_enemyShip->addPrimaryWeapon(m_weaponBuilder.build());
  m_enemyShip->setAutoShoot(true);

  Game::Movement::MovementStrategy horizontalStrategyLeft =
      Game::Movement::HorizontalMovementStrategy(200, -1);
  Game::Movement::MovementStrategy horizontalStrategyRight =
      Game::Movement::HorizontalMovementStrategy(200, 1);
  Game::Movement::MovementStrategy horizontalCombined =
      horizontalStrategyLeft + horizontalStrategyRight;

  Game::Movement::MovementStrategy verticalStrategy =
      Game::Movement::VerticalMovementStrategy(200, 1);
  Game::Movement::MovementStrategy stationaryStrategy =
      Game::Movement::StationaryMovementStrategy();
  std::vector<std::pair<Game::Movement::MovementStrategy, float>>
      verticalCombined = {std::make_pair(verticalStrategy, 0.25f),
                          std::make_pair(stationaryStrategy, 3.0f)};

  Game::Movement::IntervalMovementStrategy horizontalIntervalStrategy =
      Game::Movement::IntervalMovementStrategy(horizontalCombined, 1.0f);
  Game::Movement::IntervalMovementStrategy verticalIntervalStrategy =
      Game::Movement::IntervalMovementStrategy(verticalCombined);

  Game::Movement::MovementStrategy combined =
      horizontalIntervalStrategy + verticalIntervalStrategy;

  //    enemyShip->setMovementStrategy(
  //        Game::Movement::VerticalMovementStrategy(100, 1));

  m_enemyShip->setMovementStrategy(combined);
}

Level LevelLoader::loadLevel(const std::string &filepath) {
  qDebug() << "loading level:"
           << std::filesystem::path(filepath).filename().string();
  Level level;
  YAML::Node config = YAML::LoadFile(filepath);
  level.levelNumber = config["Level"].as<int>();
  level.name = config["Name"].as<std::string>();
  level.description = config["Description"].as<std::string>();

  // Parse and populate spawn events and other properties
  // For each spawn event in the YAML file:
  for (const auto &eventNode : config["SpawnEvents"]) {
    auto formationNode = eventNode["Formation"];
    auto positionNode = eventNode["Position"];
    float xRatio = positionNode["X"].as<float>(-1);
    float yRatio = positionNode["Y"].as<float>(-1);
    int x = static_cast<int>(xRatio * m_screenWidth);
    int y = static_cast<int>(yRatio * m_screenHeight);
    SpawnEvent event(
        [this](auto object) { m_gameState->addGameObject(object); });
    event = event.withCount(eventNode["Count"].as<int>())
                .withTriggerTime(eventNode["Time"].as<int>())
                .withInterval(eventNode["IntervalMs"].as<int>())
                .withPosition(QPoint(x, y))
                .withGameObject(m_enemyShip->clone());

    qDebug() << "adding spawn event...";
    level.spawnEvents.push_back(event);
  }

  return level;
}

std::unordered_map<int, Level> LevelLoader::loadLevels() {
  std::unordered_map<int, Level> levels;

  // Define the path to the levels directory
  std::filesystem::path levelsPath = std::filesystem::current_path() / "levels";

  // Regular expression to match files named "level_n.yaml"
  std::regex levelFilePattern("level_(\\d+)\\.yaml");

  // Check if the directory exists
  if (std::filesystem::exists(levelsPath) &&
      std::filesystem::is_directory(levelsPath)) {
    // Iterate over the files in the directory
    for (const auto &entry : std::filesystem::directory_iterator(levelsPath)) {
      const auto &path = entry.path();
      std::string filename = path.filename().string();

      // Check if the filename matches the pattern
      if (std::regex_match(filename, levelFilePattern)) {
        // Load the level and add it to the vector
        Level level = loadLevel(path.string());
        levels[level.levelNumber] = level;
      }
    }
  }

  return levels;
}

} // namespace Levels
} // namespace Game
