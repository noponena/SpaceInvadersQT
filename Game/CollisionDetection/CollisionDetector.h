#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "BVHTree.h"
#include "BruteForce.h"
#include "GameObjects/GameObject.h"
#include "Quadtree.h"
#include <memory>
#include <mutex>

namespace Game {
namespace CollisionDetection {

class CollisionDetector {
public:
  CollisionDetector(
      const std::vector<std::shared_ptr<GameObjects::GameObject>> &gameObjects,
      QRectF screenRect);
  void detectQuadTree();
  void detectBruteForce();
  void detectBVH();
  void detectBVHParallel();

private:
  using ObjectType = GameObjects::ObjectType;
  const std::vector<std::shared_ptr<GameObjects::GameObject>> &m_gameObjects;
  BruteForce m_bruteForce;
  std::unique_ptr<Quadtree> m_quadtree;
  BVHTree m_bvhTree;

  std::vector<std::pair<GameObjects::GameObject *, GameObjects::GameObject *>> m_collisionPairs;
  std::mutex m_collisionMutex;
};

} // namespace CollisionDetection
} // namespace Game

#endif // COLLISIONDETECTOR_H
