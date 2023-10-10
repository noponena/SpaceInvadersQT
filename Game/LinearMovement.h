#ifndef GAME_LINEARMOVEMENT_H
#define GAME_LINEARMOVEMENT_H

#include "MovementFunction.h"

namespace Game {

class LinearMovement : public MovementFunction
{
public:
    LinearMovement();

    // MovementFunction interface
public:
    static MovementFunc get(const float &speed, const int &direction);
};

} // namespace Game

#endif // GAME_LINEARMOVEMENT_H
