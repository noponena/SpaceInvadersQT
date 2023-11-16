#ifndef UTILS_PERFORMANCEMONITOR_H
#define UTILS_PERFORMANCEMONITOR_H

#include <fstream>

namespace Utils {

class PerformanceMonitor {
public:
  static PerformanceMonitor &getInstance(int logIntervalMs = 1000,
                                         char csvDelimiter = ';') {
    static PerformanceMonitor instance(logIntervalMs, csvDelimiter);
    return instance;
  }
  ~PerformanceMonitor();
  void logPerformance(int frameTimeMs, int gameObjectCount, int sceneItemCount);

private:
  PerformanceMonitor(int logIntervalMs, char csvDelimiter);
  std::ofstream m_outFile;
  int m_logIntervalMs;
  char m_csvDelimiter;
  int m_accumulatedTimeSinceLastLogMs;

  inline bool isTimeToLog(int frameTimeMs);

  PerformanceMonitor(const PerformanceMonitor &) = delete;
  PerformanceMonitor &operator=(const PerformanceMonitor &) = delete;
};

} // namespace Utils

#endif // UTILS_PERFORMANCEMONITOR_H
