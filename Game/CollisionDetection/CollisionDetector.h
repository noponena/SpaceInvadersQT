#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "BVHTree.h"
#include "BruteForce.h"
#include "GameObjects/GameObject.h"
#include "Quadtree.h"
#include <memory>
#include <set>

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

private:
  using ObjectType = GameObjects::ObjectType;
  const std::vector<std::shared_ptr<GameObjects::GameObject>> &m_gameObjects;
  BruteForce m_bruteForce;
  std::unique_ptr<Quadtree> m_quadtree;
  BVHTree m_bvhTree;
};

} // namespace CollisionDetection
} // namespace Game

#endif // COLLISIONDETECTOR_H
