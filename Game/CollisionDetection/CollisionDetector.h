#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "GameObjects/GameObject.h"
#include "Quadtree.h"
#include <memory>
#include <set>

class CollisionDetector {
public:
  CollisionDetector(
      const std::vector<std::shared_ptr<GameObjects::GameObject>> &gameObjects,
      QRectF screenRect);
  void detectQuadTree();
  void detect();

private:
  using ObjectType = GameObjects::ObjectType;
  const std::vector<std::shared_ptr<GameObjects::GameObject>> &m_gameObjects;
  std::unique_ptr<Quadtree> m_quadtree;
};

#endif // COLLISIONDETECTOR_H
