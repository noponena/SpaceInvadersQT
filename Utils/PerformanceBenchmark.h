#ifndef UTILS_PERFORMANCEMONITOR_H
#define UTILS_PERFORMANCEMONITOR_H

#include "GameObjects/Ships/PlayerShip.h"
#include <filesystem>
#include <fstream>

namespace Utils {

class PerformanceBenchmark {
public:
  static PerformanceBenchmark &getInstance(Config::GameContext ctx,
                                           int logIntervalMs = 0,
                                           int gameObjectThreshold = 50,
                                           char csvDelimiter = ';') {
    static PerformanceBenchmark instance(logIntervalMs, gameObjectThreshold,
                                         csvDelimiter, ctx);
    return instance;
  }
  ~PerformanceBenchmark();
  void initializeBenchmark(
      std::shared_ptr<GameObjects::Ships::PlayerShip> playerShipplayerShip);
  void recordFrameTime(int frameTimeMs);
  void logPerformanceScore();

private:
  PerformanceBenchmark(int logIntervalMs, int gameObjectThreshold,
                       char csvDelimiter, const Config::GameContext ctx);

  Config::GameContext m_gameCtx;
  std::ofstream m_outFile;
  std::filesystem::path m_filePath;
  int m_logIntervalMs;
  int m_gameObjectThreshold;
  char m_csvDelimiter;
  const float m_gain = 1000.0f;
  std::vector<float> m_frameTimesMs;

  std::vector<float> filteredFrameTimes(float maxAllowedMs = 1000.0f,
                                        size_t skipFirstN = 100) const;
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
