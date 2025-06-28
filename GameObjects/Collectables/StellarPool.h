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
  static std::shared_ptr<Stellar>
  getCollectable(const Transform &transform, const Config::GameContext &ctx) {
    if (pool.empty()) {
      // If the pool is empty, create a new one (this should ideally not happen
      // often)
      auto newCollectable = std::make_shared<Stellar>(ctx);
      newCollectable->setTransform(transform);
      newCollectable->initialize();
      return newCollectable;
    } else {
      auto collectable = pool.front();
      pool.pop();
      // collectable->reset();
      collectable->moveAbsolute(transform.position);
      qDebug() << "pool size:" << pool.size();
      return collectable;
    }
  }

  static void returnCollectable(std::shared_ptr<Stellar> collectable) {
    pool.push(collectable);
    qDebug() << "pool size:" << pool.size();
  }

  static void initializePool(int initialSize, const Config::GameContext ctx) {
    for (int i = 0; i < initialSize; ++i) {
      auto collectable = std::make_shared<Stellar>(ctx);
      collectable->initialize();
      pool.push(collectable);
    }
  }
};
} // namespace Collectables
} // namespace GameObjects

#endif // STELLARPOOL_H
