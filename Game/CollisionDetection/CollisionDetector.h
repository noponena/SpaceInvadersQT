#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "BVHTree.h"
#include "GameObjects/GameObject.h"
#include <memory>
#include <mutex>

namespace Game {
namespace CollisionDetection {

class CollisionDetector {
public:
  CollisionDetector(
      const std::vector<std::shared_ptr<GameObjects::GameObject>> &gameObjects);
  void detectQuadTree();
  void detectBruteForce();
  void detectBVH();
  void detectBVHParallel();

private:
  using ObjectType = GameObjects::ObjectType;
  const std::vector<std::shared_ptr<GameObjects::GameObject>> &m_gameObjects;
  BVHTree m_bvhTree;

  std::vector<std::pair<GameObjects::GameObject *, GameObjects::GameObject *>>
      m_collisionPairs;
  std::mutex m_collisionMutex;
};

} // namespace CollisionDetection
} // namespace Game

#endif // COLLISIONDETECTOR_H
