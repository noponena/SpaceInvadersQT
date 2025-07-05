#include "MovementStrategyLoader.h"
#include <cmath>
#include <stdexcept>

namespace Game {
namespace Movement {

MovementStrategy
MovementStrategyLoader::loadFromFile(const std::filesystem::path &path) {
  qDebug() << "[MovementStrategyLoader] Loading movement strategy from:"
           << QString::fromStdString(path.string());
  YAML::Node doc = YAML::LoadFile(path.string());
  auto strategy = loadFromYaml(doc);
  qDebug() << "[MovementStrategyLoader] Finished loading strategy from file.";
  return strategy;
}

MovementStrategy MovementStrategyLoader::loadFromYaml(const YAML::Node &node) {
  MovementStrategy strategy;
  qDebug()
      << "[MovementStrategyLoader] Parsing movement strategy from YAML node...";

  // xAxis
  if (node["xAxis"]) {
    qDebug() << "[MovementStrategyLoader] ...found xAxis movement";
    loadAxis(node["xAxis"], strategy.m_xAxisMovementStrategies);
  } else {
    qDebug() << "[MovementStrategyLoader] ...no xAxis movement found";
  }
  // yAxis
  if (node["yAxis"]) {
    qDebug() << "[MovementStrategyLoader] ...found yAxis movement";
    loadAxis(node["yAxis"], strategy.m_yAxisMovementStrategies);
  } else {
    qDebug() << "[MovementStrategyLoader] ...no yAxis movement found";
  }
  return strategy;
}

void MovementStrategyLoader::loadAxis(const YAML::Node &axisNode,
                                      std::vector<AxisMovementStrategy> &out) {
  int stepIdx = 0;
  for (const auto &stepNode : axisNode) {
    qDebug() << "[MovementStrategyLoader]   Parsing axis movement step #"
             << stepIdx++;
    out.push_back(parseStep(stepNode));
  }
}

AxisMovementStrategy MovementStrategyLoader::parseStep(const YAML::Node &node) {
  const std::string type = node["type"].as<std::string>();
  qDebug() << "[MovementStrategyLoader]     Parsing axis step of type:"
           << QString::fromStdString(type);

  if (type == "Linear") {
    float speed = node["speed"].as<float>();
    int direction = node["direction"] ? node["direction"].as<int>() : 1;
    bool updateAnchor =
        node["updateAnchor"] ? node["updateAnchor"].as<bool>() : false;
    qDebug() << "[MovementStrategyLoader]     Linear: speed =" << speed
             << "direction =" << direction << "updateAnchor =" << updateAnchor;
    return LinearMovement(speed, direction, updateAnchor);

  } else if (type == "Sinusoid") {
    float amplitude = node["amplitude"].as<float>();
    float frequency = node["frequency"].as<float>();
    float phase = node["phase"] ? node["phase"].as<float>() : 0.f;
    int direction = node["direction"] ? node["direction"].as<int>() : 1;
    bool updateAnchor =
        node["updateAnchor"] ? node["updateAnchor"].as<bool>() : false;
    qDebug() << "[MovementStrategyLoader]     Sinusoid: amplitude ="
             << amplitude << "frequency =" << frequency << "phase =" << phase
             << "direction =" << direction << "updateAnchor =" << updateAnchor;
    return SinusoidMovement(amplitude, frequency, phase, direction,
                            updateAnchor);

  } else if (type == "Stationary") {
    qDebug() << "[MovementStrategyLoader]     Stationary movement";
    return StationaryMovement();

  } else if (type == "Interval") {
    qDebug() << "[MovementStrategyLoader]     Interval movement";
    return parseInterval(node, /*isXAxis=*/true);
  } else {
    qDebug()
        << "[MovementStrategyLoader]     ERROR: Unknown axis movement type:"
        << QString::fromStdString(type);
    throw std::runtime_error("Unknown axis movement type: " + type);
  }
}

IntervalMovement MovementStrategyLoader::parseInterval(const YAML::Node &node,
                                                       bool /*isXAxis*/) {
  std::vector<float> intervals;
  if (node["intervals"]) {
    qDebug() << "[MovementStrategyLoader]       Parsing multiple intervals";
    for (const auto &v : node["intervals"]) {
      intervals.push_back(v.as<float>());
    }
  } else if (node["interval"]) {
    qDebug() << "[MovementStrategyLoader]       Parsing single interval";
    intervals.push_back(node["interval"].as<float>());
  } else {
    qDebug() << "[MovementStrategyLoader]       ERROR: Interval missing";
    throw std::runtime_error(
        "Interval movement missing 'interval' or 'intervals'");
  }

  std::vector<AxisMovementStrategy> subStrategies;
  if (!node["strategies"] || !node["strategies"].IsSequence()) {
    qDebug() << "[MovementStrategyLoader]       ERROR: Missing or invalid "
                "strategies";
    throw std::runtime_error(
        "Interval movement must have a sequence of 'strategies'");
  }
  int stratIdx = 0;
  for (const auto &step : node["strategies"]) {
    qDebug() << "[MovementStrategyLoader]       Parsing sub-strategy #"
             << stratIdx++;
    subStrategies.push_back(parseStep(step));
  }
  qDebug()
      << "[MovementStrategyLoader]       IntervalMovement: strategies count ="
      << subStrategies.size() << "intervals count =" << intervals.size();
  return IntervalMovement(subStrategies, intervals);
}

} // namespace Movement
} // namespace Game
