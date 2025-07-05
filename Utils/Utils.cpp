#include "Utils.h"
#include <QColor>
#include <QRandomGenerator>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

namespace Utils {

float lerp(float a, float b, float t) { return a + t * (b - a); }

int randi(int lowerBound, int upperBound) {
  static std::random_device rd;
  static std::mt19937 eng(rd());
  static int prevLower = 0, prevUpper = -1;
  static std::uniform_int_distribution<> distr;
  if (lowerBound != prevLower || upperBound != prevUpper) {
    distr = std::uniform_int_distribution<>(lowerBound, upperBound);
    prevLower = lowerBound;
    prevUpper = upperBound;
  }
  return distr(eng);
}

bool probabilityCheck(float probability) {
  if (probability < 0.0f || probability > 1.0f) {
    std::cerr << "Probability out of range. It should be between 0 and 1."
              << std::endl;
    return false;
  }
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
  return dis(gen) < probability;
}

std::string getTimestampStr() {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  std::tm *broken_down_time = std::localtime(&now_c);
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(4) << broken_down_time->tm_year + 1900
     << '-' << std::setw(2) << broken_down_time->tm_mon + 1 << '-'
     << std::setw(2) << broken_down_time->tm_mday << 'T' << std::setw(2)
     << broken_down_time->tm_hour << ':' << std::setw(2)
     << broken_down_time->tm_min << ':' << std::setw(2)
     << broken_down_time->tm_sec;

  return ss.str();
}

std::filesystem::path getDataFolderPath(GameDataType type) {
  switch (type) {
  case GameDataType::ROOT:
    return std::filesystem::current_path() / "gamedata";
  case GameDataType::LEVEL:
    return std::filesystem::current_path() / "gamedata/levels";
  case GameDataType::BLUEPRINT:
    return std::filesystem::current_path() / "gamedata/blueprints";
  }

  throw std::invalid_argument("Invalid GameDataType!");
}

QColor getRandomColor() {
  int r = QRandomGenerator::global()->bounded(256);
  int g = QRandomGenerator::global()->bounded(256);
  int b = QRandomGenerator::global()->bounded(256);

  return QColor(r, g, b);
}

} // namespace Utils
