#ifndef MATHFUNCTIONS_H
#define MATHFUNCTIONS_H

#include <algorithm>
#include <vector>

namespace Utils::MathFunctions {
float percentile(std::vector<float> vec, float p) {
  if (vec.empty())
    return 0.0f;
  std::sort(vec.begin(), vec.end());
  float idx = p * (vec.size() - 1);
  size_t i = static_cast<size_t>(idx);
  float frac = idx - i;
  if (i + 1 < vec.size())
    return vec[i] * (1 - frac) + vec[i + 1] * frac;
  else
    return vec[i];
}

template <typename T> T moveTowardZero(T value, T step) {
  if (step < 0)
    step = -step;

  if (value > 0)
    return std::max(value - step, T(0));
  else if (value < 0)
    return std::min(value + step, T(0));
  else
    return 0;
}

} // namespace Utils::MathFunctions

#endif // MATHFUNCTIONS_H
