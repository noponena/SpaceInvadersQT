#ifndef MOVEMENTFUNCTION_H
#define MOVEMENTFUNCTION_H

#include <functional>

class MovementFunction
{
public:
    using MovementFunc = std::function<int(int, int)>;
    MovementFunction();
    static MovementFunc get(const float &speed, const int &direction) {};
};

#endif // MOVEMENTFUNCTION_H
