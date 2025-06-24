#include "Utils.h"
#include <iostream>
#include <random>

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

} // namespace Utils
