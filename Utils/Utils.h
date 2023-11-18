#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <random>

namespace Utils {

float lerp(float a, float b, float t);
int randi(int lowerBound, int upperBound);
bool probabilityCheck(float probability);

} // namespace Utils

#endif // UTILS_H
