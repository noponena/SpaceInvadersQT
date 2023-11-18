#include "Utils.h"
#include <iostream>
#include <random>

namespace Utils {

float lerp(float a, float b, float t) { return a + t * (b - a); }

int randi(int lowerBound, int upperBound) {
  static std::random_device rd;
  static std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(lowerBound, upperBound);
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
  std::uniform_real_distribution<> dis(0.0, 1.0);

  return dis(gen) < probability;
}

} // namespace Utils
