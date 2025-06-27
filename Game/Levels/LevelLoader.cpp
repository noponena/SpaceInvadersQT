#include "LevelLoader.h"
#include "Config/GameContext.h"
#include "Weapons/PrimaryWeapon.h"
#include <QDebug>
#include <algorithm>
#include <filesystem>
#include <regex>
#include <stdexcept>
#include <yaml-cpp/yaml.h>

namespace Game {
namespace Levels {

bool iequals(const std::string &a, const std::string &b) {
  return a.size() == b.size() &&
         std::equal(a.begin(), a.end(), b.begin(), [](char a, char b) {
           return std::tolower(a) == std::tolower(b);
         });
}

void LevelLoader::initialize() {
  if (!m_gameCtx) {
    throw std::invalid_argument("Game context has not been set.");
  }

  GameObjects::Transform transform;
  transform.colliderSize = QVector2D(30, 30);

  GameObjects::RenderDataMap renderDataMap{
      {GameObjects::RenderState::Normal,
       GameObjects::RenderData({30, 30},
                               ":/Images/enemy_laser_projectile.png")}};

  std::unique_ptr<GameObjects::Projectiles::Projectile> projectile =
      m_projectileBuilder
          .createProjectile<GameObjects::Projectiles::Projectile>(*m_gameCtx)
          .withDamage(1)
          .withMovementStrategy(
              Game::Movement::VerticalMovementStrategy(500, 1))
          .withGrahpics(renderDataMap)
          .withSpawnSound(Audio::SoundInfo(
              {true, Game::Audio::SoundEffect::LESSER_ENEMY_LASER}))
          .withObjectType(GameObjects::ObjectType::ENEMY_PROJECTILE)
          .build();

  m_weaponBuilder.createWeapon<Weapons::PrimaryWeapon>()
      .withProjectile(std::move(projectile))
      .withWeaponCooldownMs(2500);

  m_enemyShip =
      std::make_unique<GameObjects::Ships::EnemyShip>(5, transform, *m_gameCtx);
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
  try {
    qDebug() << "loading level:"
             << QString::fromStdString(
                    std::filesystem::path(filepath).filename().string());
    Level level;
    YAML::Node config = YAML::LoadFile(filepath);

    level.levelNumber = config["Level"].as<int>();
    level.name = config["Name"].as<std::string>();
    level.description = config["Description"].as<std::string>();
    level.enemyLimit = config["EnemyLimit"].as<int>(1);
    qDebug() << "name:" << QString::fromStdString(level.name);
    qDebug() << "enemyLimit:" << level.enemyLimit;
    qDebug();

    for (const auto &eventNode : config["SpawnEvents"]) {
      if (!eventNode["Enabled"].as<bool>(true))
        continue;
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
      formation =
          formation.withType(formationType)
              .withSize(formationWidth, formationHeight)
              .withSolidity(formationSolidity)
              .withSpacing(QVector2D(formationSpacingX, formationSpacingY));
      auto positionNode = eventNode["Position"];
      QVector2D lowerLimit, upperLimit;

      if (positionNode["Min"] && positionNode["Max"]) {
        auto minNode = positionNode["Min"];
        auto maxNode = positionNode["Max"];

        float minXRatio = minNode["X"].as<float>();
        float minYRatio = minNode["Y"].as<float>();
        float maxXRatio = maxNode["X"].as<float>();
        float maxYRatio = maxNode["Y"].as<float>();

        qDebug() << "minXRatio=" << minXRatio << " minYRatio=" << minYRatio
                 << " maxXRatio=" << maxXRatio << " maxYRatio=" << maxYRatio;
        qDebug() << "screenGeometry=" << m_gameCtx->screenGeometry;

        lowerLimit = QVector2D(
            static_cast<int>(minXRatio * m_gameCtx->screenGeometry.width()),
            static_cast<int>(minYRatio * m_gameCtx->screenGeometry.height()));
        upperLimit = QVector2D(
            static_cast<int>(maxXRatio * m_gameCtx->screenGeometry.width()),
            static_cast<int>(maxYRatio * m_gameCtx->screenGeometry.height()));

        qDebug() << "lowerLimit=" << lowerLimit << " upperLimit=" << upperLimit;
      } else {
        float xRatio = positionNode["X"].as<float>();
        float yRatio = positionNode["Y"].as<float>();

        lowerLimit = QVector2D(
            static_cast<int>(xRatio * m_gameCtx->screenGeometry.width()),
            static_cast<int>(yRatio * m_gameCtx->screenGeometry.height()));
        upperLimit = lowerLimit;
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
  } catch (const std::exception &ex) {
    qCritical() << "Failed to load level from file:"
                << QString::fromStdString(filepath) << "\nReason:" << ex.what();
    return Level{};
  }
}

std::map<int, Level> LevelLoader::loadLevels() {
  std::map<int, Level> levels;

  std::filesystem::path levelsPath = std::filesystem::current_path() / "levels";
  qDebug() << "Attempting to load levels from path:"
           << QString::fromStdString(levelsPath.string());

  std::regex levelFilePattern("level_(\\d+)\\.yaml");

  if (std::filesystem::exists(levelsPath) &&
      std::filesystem::is_directory(levelsPath)) {
    for (const auto &entry : std::filesystem::directory_iterator(levelsPath)) {
      const auto &path = entry.path();
      std::string filename = path.filename().string();

      if (std::regex_match(filename, levelFilePattern)) {
        Level level = loadLevel(path.string());
        if (level.levelNumber >= 0 && !level.name.empty())
          levels[level.levelNumber] = level;
        else
          qWarning() << "Level file" << QString::fromStdString(filename)
                     << "is invalid and was skipped.";
      }
    }
  }

  return levels;
}

Level LevelLoader::loadBenchmarkLevel() {
  std::filesystem::path levelsPath = std::filesystem::current_path() / "levels";
  qDebug() << "Looking for benchmark.yaml in path:"
           << QString::fromStdString(levelsPath.string());

  if (std::filesystem::exists(levelsPath) &&
      std::filesystem::is_directory(levelsPath)) {
    for (const auto &entry : std::filesystem::directory_iterator(levelsPath)) {
      const auto &path = entry.path();
      std::string filename = path.filename().string();

      // Case-insensitive comparison for "benchmark.yaml"
      if (iequals(filename, "benchmark.yaml")) {
        Level level = loadLevel(path.string());
        if (!level.name.empty()) {
          return level;
        } else {
          qWarning() << "benchmark.yaml exists but is invalid.";
          break;
        }
      }
    }
  }

  qWarning() << "benchmark.yaml not found in levels directory.";
  return Level{};
}

void LevelLoader::setGameCtx(Config::GameContext &ctx) {
  m_gameCtx = std::make_unique<Config::GameContext>(ctx);
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
