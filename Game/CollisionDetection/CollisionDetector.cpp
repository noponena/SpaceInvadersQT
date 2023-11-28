#include "CollisionDetector.h"
#include <set>

namespace Game {
namespace CollisionDetection {

CollisionDetector::CollisionDetector(
    const std::vector<std::shared_ptr<GameObjects::GameObject>> &gameObjects,
    QRectF screenRect)
    : m_gameObjects(gameObjects) {
  m_quadtree = std::make_unique<Quadtree>(0, screenRect);
}

void CollisionDetector::detectQuadTree() {
  // Clear the quadtree for fresh insertion of objects.
  m_quadtree->clear();

  // Insert all game objects into the quadtree.
  for (const auto &object : m_gameObjects) {
    m_quadtree->insert(object.get());
  }

  // Check for potential collisions.
  std::set<std::pair<uint64_t, uint64_t>> checkedPairs;

  for (const auto &object : m_gameObjects) {
    auto potentialCollisions = m_quadtree->query(object.get());

    for (auto &potentialObject : potentialCollisions) {
      auto sortedPair =
          (object->id() < potentialObject->id())
              ? std::make_pair(object->id(), potentialObject->id())
              : std::make_pair(potentialObject->id(), object->id());

      if (checkedPairs.find(sortedPair) == checkedPairs.end()) {
        if (object->isCollidingWith(*potentialObject)) {
          object->collide(*potentialObject);
        }
        checkedPairs.insert(sortedPair);
      }
    }
  }

  checkedPairs.clear();
}

void CollisionDetector::detectBVH() {
  std::vector<GameObjects::GameObject *> vec;
  vec.reserve(m_gameObjects.size());
  for (auto const &object : m_gameObjects) {
    vec.push_back(object.get());
  }

  std::set<std::pair<uint64_t, uint64_t>> checkedPairs;
  std::shared_ptr<BVHNode> node = m_bvhTree.build(vec);

  for (auto const &object : m_gameObjects) {
    m_bvhTree.clearProcessedPairs();
    std::vector<GameObjects::GameObject *> collisionObjects =
        m_bvhTree.query(node, object.get());
    for (auto const &collisionObject : collisionObjects) {
      object->collide(*collisionObject);
    }
  }
}

void CollisionDetector::detectBruteForce() {
  m_bruteForce.detect(m_gameObjects);
}

} // namespace CollisionDetection
} // namespace Game
