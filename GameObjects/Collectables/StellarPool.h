#ifndef STELLARPOOL_H
#define STELLARPOOL_H

#include "Stellar.h"
#include <memory>
#include <queue>

namespace GameObjects {
namespace Collectables {
class StellarPool {
private:
    static std::queue<std::shared_ptr<Stellar>> pool;

public:
    static std::shared_ptr<Stellar> getCollectable(const Position& position) {
        if (pool.empty()) {
            // If the pool is empty, create a new one (this should ideally not happen often)
            auto newCollectable = std::make_shared<Stellar>(position);
            newCollectable->initialize();
            return newCollectable;
        } else {
            auto collectable = pool.front();
            pool.pop();
            collectable->reset();
            collectable->setPosition(position);
            qDebug() << "pool size:" << pool.size();
            return collectable;
        }
    }

    static void returnCollectable(std::shared_ptr<Stellar> collectable) {
        pool.push(collectable);
        qDebug() << "pool size:" << pool.size();
    }

    static void initializePool(int initialSize) {
        for (int i = 0; i < initialSize; ++i) {
            auto collectable = std::make_shared<Stellar>(Position(0, 0));
            collectable->initialize();
            pool.push(collectable);
        }
    }
};
}
}



#endif // STELLARPOOL_H
