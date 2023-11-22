#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "GameObjects/GameObject.h"
#include "Quadtree.h"
#include "BVHTree.h"
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
    void detect();

    void detectBVH();
private:
    using ObjectType = GameObjects::ObjectType;
    const std::vector<std::shared_ptr<GameObjects::GameObject>> &m_gameObjects;
    std::unique_ptr<Quadtree> m_quadtree;
    BVHTree m_bvhTree;
};

}
}


#endif // COLLISIONDETECTOR_H
