#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "Quadtree.h"
#include "GameObjects/GameObject.h"
#include <memory>
#include <set>


class CollisionDetector
{
public:
    CollisionDetector(const std::list<std::unique_ptr<GameObjects::GameObject>>& gameObjects);
    void detectQuadTree();
    void detect();

private:
    using ObjectType = GameObjects::ObjectType;
    const std::list<std::unique_ptr<GameObjects::GameObject>>& m_gameObjects;
    std::unique_ptr<Quadtree> m_quadtree;
};

#endif // COLLISIONDETECTOR_H
