#include "PerformanceMonitor.h"
#include <QDebug>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace Utils {

PerformanceMonitor::PerformanceMonitor(int logIntervalMs, char csvDelimiter)
    : m_logIntervalMs(logIntervalMs), m_csvDelimiter(csvDelimiter),
      m_accumulatedTimeSinceLastLogMs(0) {
  std::filesystem::path exePath = std::filesystem::current_path();
  std::filesystem::path performanceDir = exePath / "performance";
  std::filesystem::create_directory(performanceDir);

  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y-%m-%dT%H_%M_%S");
  std::string filename = ss.str() + ".csv";
  std::filesystem::path filePath = performanceDir / filename;

  m_outFile.open(filePath, std::ios::out | std::ios::app);
  if (m_outFile.is_open()) {
    m_outFile << "timestamp" << m_csvDelimiter << "frametime" << m_csvDelimiter
              << "game_objects" << m_csvDelimiter << "scene_items"
              << m_csvDelimiter << "frametime_per_game_object" << m_csvDelimiter
              << "frametime_per_scene_item\n";
  }
}

PerformanceMonitor::~PerformanceMonitor() {
  if (m_outFile.is_open()) {
    m_outFile.close();
  }
}

bool PerformanceMonitor::isTimeToLog(int frameTimeMs) {
  m_accumulatedTimeSinceLastLogMs += frameTimeMs;
  return m_accumulatedTimeSinceLastLogMs >= m_logIntervalMs;
}

void PerformanceMonitor::logPerformance(int frameTimeMs, int gameObjectCount,
                                        int sceneItemCount) {
  if (isTimeToLog(frameTimeMs)) {
    if (m_outFile.is_open()) {
      float frameTimePerGameObject =
          gameObjectCount > 0 ? static_cast<float>(frameTimeMs) /
                                    static_cast<float>(gameObjectCount)
                              : 0.0f;

      float frameTimePerSceneItem = sceneItemCount > 0
                                        ? static_cast<float>(frameTimeMs) /
                                              static_cast<float>(sceneItemCount)
                                        : 0.0f;

      auto now = std::chrono::system_clock::now();
      auto now_c = std::chrono::system_clock::to_time_t(now);
      std::stringstream ss;
      ss << std::put_time(std::localtime(&now_c), "%H:%M:%S");

      m_outFile << ss.str() << m_csvDelimiter << frameTimeMs << m_csvDelimiter
                << gameObjectCount << m_csvDelimiter << sceneItemCount
                << m_csvDelimiter << frameTimePerGameObject << m_csvDelimiter
                << frameTimePerSceneItem << '\n';
    }
    m_accumulatedTimeSinceLastLogMs = 0;
  }
}

} // namespace Utils
