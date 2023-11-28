#ifndef GAME_COLLISIONDETECTION_BVHNODE_H
#define GAME_COLLISIONDETECTION_BVHNODE_H

#include "GameObjects/GameObject.h"

namespace Game {
namespace CollisionDetection {

class BVHNode {
public:
  BVHNode *left;
  BVHNode *right;
  // BoundingBox boundingBox;
  std::vector<GameObjects::GameObject *> objects; // Only for leaf nodes

  BVHNode(std::vector<GameObjects::GameObject *> &objects);
  ~BVHNode();

  void split(); // Implement your splitting heuristic here
  bool intersects(const GameObjects::GameObject &object) const;
  std::vector<GameObjects::GameObject *>
  query(const GameObjects::GameObject &object) const;
};

} // namespace CollisionDetection
} // namespace Game

#endif // GAME_COLLISIONDETECTION_BVHNODE_H
