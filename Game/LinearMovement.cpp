#include "LinearMovement.h"

namespace Game {

LinearMovement::LinearMovement()
{

}

MovementFunction::MovementFunc LinearMovement::get(const float &speed, const int &direction)
{
    return [speed, direction](int currentPosition, int deltaTime) {
        return currentPosition + (speed * deltaTime * direction);
    };
}

} // namespace Game
