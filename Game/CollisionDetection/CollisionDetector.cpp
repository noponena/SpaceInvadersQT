#include "CollisionDetector.h"
#include <set>

CollisionDetector::CollisionDetector(const std::list<std::shared_ptr<GameObjects::GameObject>>& gameObjects):
    m_gameObjects(gameObjects)
{
    m_quadtree = std::make_unique<Quadtree>(0, QRectF{0, 0, 1920, 1080});
}

void CollisionDetector::detectQuadTree() {
    // Counter for the number of collision checks performed.
    int collisionCheckCount = 0;

    // Clear the quadtree for fresh insertion of objects.
    m_quadtree->clear();

    // Insert all game objects into the quadtree.
    for (const auto& object : m_gameObjects) {
        m_quadtree->insert(object);
    }

    // Check for potential collisions.
    std::set<std::pair<long long unsigned, long long unsigned>> checkedPairs;

    for (const auto& object : m_gameObjects) {
        auto potentialCollisions = m_quadtree->query(object);

        for (auto& potentialObject : potentialCollisions) {
            auto sortedPair = (object->id() < potentialObject->id())
                              ? std::make_pair(object->id(), potentialObject->id())
                              : std::make_pair(potentialObject->id(), object->id());

            if (checkedPairs.find(sortedPair) == checkedPairs.end()) {
                collisionCheckCount++;
                if (object->isCollidingWith(*potentialObject)) {
                    object->collide(*potentialObject);
                }
                checkedPairs.insert(sortedPair);
            }
        }
    }

    checkedPairs.clear();
    //std::cout << "Collision checks performed: " << collisionCheckCount << std::endl;
}

void CollisionDetector::detect()
{
    // Counter for the number of collision checks performed.
    int collisionCheckCount = 0;

    for (auto it1 = m_gameObjects.begin(); it1 != m_gameObjects.end(); ++it1) {
      for (auto it2 = std::next(it1); it2 != m_gameObjects.end(); ++it2) {
        // Increment the counter before checking for collision.
        collisionCheckCount++;
        if ((*it1)->isCollidingWith(**it2)) {
          (*it1)->collide(**it2);
        }
      }
    }

    // Output the number of collision checks performed.
    //std::cout << "Collision checks performed: " << collisionCheckCount << std::endl;
}


