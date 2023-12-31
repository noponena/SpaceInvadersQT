#include "LevelLoader.h"
#include "Weapons/PrimaryWeapon.h"
#include <QDebug>
#include <filesystem>
#include <regex>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Levels {

LevelLoader::LevelLoader() {}

void LevelLoader::initialize() {
  GameObjects::Position pos(0, 0, m_minX, m_maxX, m_minY, m_maxY);

  GameObjects::PixmapData pixmapData{
      QPointF(30, 30), ":/Images/enemy_laser_projectile.png", "", ""};

  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      m_projectileBuilder
          .createProjectile<GameObjects::Projectiles::Projectile>()
          .withDamage(1)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(500, 1))
          .withGrahpics(pixmapData)
          .withSpawnSound(Audio::SoundInfo(
              {true, Game::Audio::SoundEffect::LESSER_ENEMY_LASER}))
          .withObjectType(GameObjects::ObjectType::ENEMY_PROJECTILE)
          .build();

  m_weaponBuilder.createWeapon<Weapons::PrimaryWeapon>()
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
  level.enemyLimit = config["EnemyLimit"].as<int>(1);

  // Parse and populate spawn events and other properties
  // For each spawn event in the YAML file:
  for (const auto &eventNode : config["SpawnEvents"]) {
    if (!eventNode["Enabled"].as<bool>(true)) {
      continue;
    }
    auto formationNode = eventNode["Formation"];
    Formation::Type formationType =
        stringToFormationType(formationNode["Type"].as<std::string>());
    int formationWidth = formationNode["Width"].as<int>();
    int formationHeight = formationNode["Height"].as<int>();
    bool formationSolidity = formationNode["Solid"].as<bool>();
    auto formationSpacingNode = formationNode["Spacing"];
    int formationSpacingX = formationSpacingNode["X"].as<int>();
    int formationSpacingY = formationSpacingNode["Y"].as<int>();
    Formation formation;
    formation = formation.withType(formationType)
                    .withSize(formationWidth, formationHeight)
                    .withSolidity(formationSolidity)
                    .withSpacing(QPoint(formationSpacingX, formationSpacingY));
    auto positionNode = eventNode["Position"];
    QPoint lowerLimit, upperLimit;

    if (positionNode["Min"] && positionNode["Max"]) {
      // Position range is specified
      auto minNode = positionNode["Min"];
      auto maxNode = positionNode["Max"];

      float minXRatio = minNode["X"].as<float>();
      float minYRatio = minNode["Y"].as<float>();
      float maxXRatio = maxNode["X"].as<float>();
      float maxYRatio = maxNode["Y"].as<float>();

      lowerLimit = QPoint(static_cast<int>(minXRatio * m_screenWidth),
                          static_cast<int>(minYRatio * m_screenHeight));
      upperLimit = QPoint(static_cast<int>(maxXRatio * m_screenWidth),
                          static_cast<int>(maxYRatio * m_screenHeight));
    } else {
      // Single position is specified
      float xRatio = positionNode["X"].as<float>();
      float yRatio = positionNode["Y"].as<float>();

      lowerLimit = QPoint(static_cast<int>(xRatio * m_screenWidth),
                          static_cast<int>(yRatio * m_screenHeight));
      upperLimit = lowerLimit; // Use the same point for both limits
    }
    SpawnEvent event;
    event = event.withCount(eventNode["Count"].as<int>())
                .withTriggerTime(eventNode["Time"].as<int>())
                .withInterval(eventNode["IntervalMs"].as<int>())
                .withPositionRange(lowerLimit, upperLimit)
                .withFormation(formation)
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

void LevelLoader::setScreenSize(QPoint screenSize) {
  m_screenWidth = screenSize.x();
  m_screenHeight = screenSize.y();
}

void LevelLoader::setPositionConstraints(QPoint positionConstraintMin,
                                         QPoint positionConstraintMax) {
  m_minX = positionConstraintMin.x();
  m_maxX = positionConstraintMax.x();
  m_minY = positionConstraintMin.y();
  m_maxY = positionConstraintMax.y();
}

Formation::Type
LevelLoader::stringToFormationType(std::string formationTypeStr) {
  std::string upperStr = formationTypeStr;
  std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
                 [](unsigned char c) { return std::toupper(c); });

  if (upperStr == "RECTANGLE")
    return Formation::Type::RECTANGLE;
  if (upperStr == "TRIANGLE")
    return Formation::Type::TRIANGLE;
  if (upperStr == "CIRCLE")
    return Formation::Type::CIRCLE;

  throw std::invalid_argument("Unrecognized formation type: '" +
                              formationTypeStr + "'");
}

} // namespace Levels
} // namespace Game
