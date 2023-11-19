#ifndef UTILS_PERFORMANCEMONITOR_H
#define UTILS_PERFORMANCEMONITOR_H

#include "GameObjects/Ships/PlayerShip.h"
#include <filesystem>
#include <fstream>

namespace Utils {

class PerformanceBenchmark {
public:
  static PerformanceBenchmark &getInstance(int logIntervalMs = 0,
                                           int gameObjectThreshold = 500,
                                           char csvDelimiter = ';') {
    static PerformanceBenchmark instance(logIntervalMs, gameObjectThreshold,
                                         csvDelimiter);
    return instance;
  }
  ~PerformanceBenchmark();
  void initializeBenchmark(GameObjects::Ships::PlayerShip *playerShip);
  void logPerformance(int frameTimeMs, int gameObjectCount, int sceneItemCount);
  void logPerformanceScore();

private:
  PerformanceBenchmark(int logIntervalMs, int gameObjectThreshold,
                       char csvDelimiter);
  std::ofstream m_outFile;
  std::filesystem::path m_filePath;
  int m_logIntervalMs;
  int m_gameObjectThreshold;
  char m_csvDelimiter;
  int m_accumulatedTimeSinceLastLogMs;
  const float m_gain = 1000.0f;
  std::vector<std::tuple<int, int, int, float, float>> m_stats;

  float calculateBenchmarkScore();
  float getMemUsage();

  inline void createFile();
  inline void openFile();
  inline void closeFile();
  inline void writeHeader();
  inline bool isTimeToLog(int frameTimeMs);

  PerformanceBenchmark(const PerformanceBenchmark &) = delete;
  PerformanceBenchmark &operator=(const PerformanceBenchmark &) = delete;
};

} // namespace Utils

#endif // UTILS_PERFORMANCEMONITOR_H
