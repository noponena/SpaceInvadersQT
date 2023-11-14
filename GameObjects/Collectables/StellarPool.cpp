#include "StellarPool.h"

// Static member definition
std::queue<std::shared_ptr<GameObjects::Collectables::Stellar>>
    GameObjects::Collectables::StellarPool::pool;
