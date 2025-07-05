#include "CollisionDetector.h"
#include <QtConcurrent>
#include <set>

namespace Game {
namespace CollisionDetection {

CollisionDetector::CollisionDetector(
    const std::vector<std::shared_ptr<GameObjects::GameObject>> &gameObjects,
    QRectF screenRect)
    : m_gameObjects(gameObjects) {}

void CollisionDetector::detectBVH() {
  std::vector<GameObjects::GameObject *> vec;
  vec.reserve(m_gameObjects.size());
  for (auto const &object : m_gameObjects) {
    vec.push_back(object.get());
  }

  std::shared_ptr<BVHNode> node = m_bvhTree.build(vec);

  for (auto const &object : m_gameObjects) {
    m_bvhTree.clearProcessedPairs();
    std::set<std::pair<std::uint64_t, std::uint64_t>> processedPairs;
    std::vector<GameObjects::GameObject *> collisionObjects;
    collisionObjects.reserve(16);
    m_bvhTree.query(node, object.get(), processedPairs, collisionObjects);
    for (auto const &collisionObject : collisionObjects) {
      object->collide(*collisionObject);
    }
  }
}

void CollisionDetector::detectBVHParallel() {
  std::vector<GameObjects::GameObject *> vec;
  vec.reserve(m_gameObjects.size());

  for (auto const &object : m_gameObjects) {
    vec.push_back(object.get());
  }

  std::shared_ptr<BVHNode> node = m_bvhTree.build(vec);
  m_collisionPairs.reserve(m_gameObjects.size() * 4);

  QtConcurrent::blockingMap(
      m_gameObjects,
      [&node, this](const std::shared_ptr<GameObjects::GameObject> &object) {
        std::set<std::pair<std::uint64_t, std::uint64_t>> processedPairs;
        std::vector<GameObjects::GameObject *> collisionObjects;
        collisionObjects.reserve(16);
        m_bvhTree.query(node, object.get(), processedPairs, collisionObjects);
        for (auto const &collisionObject : collisionObjects) {
          auto id1 = object->id(), id2 = collisionObject->id();
          auto pair = (id1 < id2)
                          ? std::make_pair(object.get(), collisionObject)
                          : std::make_pair(collisionObject, object.get());
          {
            std::lock_guard<std::mutex> lock(m_collisionMutex);
            m_collisionPairs.push_back(pair);
          }
        }
      });

  std::set<std::pair<uint64_t, uint64_t>> uniquePairs;
  for (auto &pair : m_collisionPairs) {
    auto id1 = pair.first->id(), id2 = pair.second->id();
    auto sortedPair =
        (id1 < id2) ? std::make_pair(id1, id2) : std::make_pair(id2, id1);
    if (uniquePairs.insert(sortedPair).second) {
      pair.first->collide(*pair.second);
    }
  }

  m_collisionPairs.clear();
}

} // namespace CollisionDetection
} // namespace Game
