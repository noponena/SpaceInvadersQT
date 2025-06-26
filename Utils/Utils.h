#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {

float lerp(float a, float b, float t);
int randi(int lowerBound, int upperBound);
bool probabilityCheck(float probability);
std::string getTimestampStr();

} // namespace Utils

#endif // UTILS_H
