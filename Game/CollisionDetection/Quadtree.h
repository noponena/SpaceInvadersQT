#ifndef QUADTREE_H
#define QUADTREE_H

#include "GameObjects/GameObject.h"
#include <iostream>
#include <memory>
#include <set>
#include <vector>

namespace Game {
namespace CollisionDetection {
class Quadtree {
public:
    Quadtree(int pLevel, QRectF pBounds) : m_level(pLevel), m_bounds(pBounds) {
        for (int i = 0; i < 4; i++) {
            m_nodes.push_back(nullptr);
        }
    }

    inline void clear() {
        m_objects.clear();
        for (auto &subnode : m_nodes) {
            if (subnode) {
                subnode->clear();
                subnode.reset();
            }
        }
    }

    inline void insert(GameObjects::GameObject *object) {
        // If the node has subnodes, attempt to insert the object into them
        if (m_nodes[0]) {
            int index = getIndex(object->getBoundingBox());
            if (index != -1) {
                m_nodes[index]->insert(object);
                return;
            }
            // For objects that span multiple quadrants, continue and add to this node
        }

        // Add the object to this node
        m_objects.emplace_back(object);

        // If this node exceeds capacity and hasn't reached max level, split and redistribute objects
        if (m_objects.size() > MAX_OBJECTS && m_level < MAX_LEVELS) {
            if (!m_nodes[0]) {
                split();
            }
            uint64_t i = 0;
            while (i < m_objects.size()) {
                int index = getIndex(m_objects[i]->getBoundingBox());
                if (index != -1) {
                    m_nodes[index]->insert(m_objects[i]);
                    m_objects.erase(m_objects.begin() + i);
                } else {
                    i++;
                }
            }
        }
    }

    std::vector<GameObjects::GameObject *> inline query(
        const GameObjects::GameObject *queryObject) const {
        QRectF range = queryObject->getBoundingBox();
        std::vector<GameObjects::GameObject *> possibleCollisions;

        if (!m_bounds.intersects(range)) {
            return possibleCollisions;
        }

        for (const auto &object : m_objects) {
            if (canCollide(queryObject->objectTypes(), object->objectTypes())) {
                QRectF objectBounds = object->getBoundingBox();
                if (range.intersects(objectBounds)) {
                    possibleCollisions.push_back(object);
                }
            }
        }

        if (m_nodes[0]) {
            for (const auto &subnode : m_nodes) {
                auto subnodeCollisions = subnode->query(queryObject);
                possibleCollisions.insert(possibleCollisions.end(),
                                          subnodeCollisions.begin(),
                                          subnodeCollisions.end());
            }
        }

        return possibleCollisions;
    }

private:
    static constexpr int MAX_OBJECTS = 20;
    static constexpr int MAX_LEVELS = 5;

    int m_level;
    QRectF m_bounds;
    std::vector<GameObjects::GameObject *> m_objects;
    std::vector<std::unique_ptr<Quadtree>> m_nodes;
    using ObjectType = GameObjects::ObjectType;
    std::unordered_map<GameObjects::ObjectType,
                       std::unordered_set<GameObjects::ObjectType>>
        m_collisionMap{
                       {ObjectType::PLAYER_SHIP,
                        {ObjectType::ENEMY_PROJECTILE, ObjectType::COLLECTABLE}},

                       {ObjectType::ENEMY_SHIP, {ObjectType::PLAYER_PROJECTILE}},
                       };

    inline void split() {
        float subWidth = m_bounds.width() / 2;
        float subHeight = m_bounds.height() / 2;
        float x = m_bounds.x();
        float y = m_bounds.y();

        m_nodes[0] = std::make_unique<Quadtree>(
            m_level + 1, QRectF{x + subWidth, y, subWidth, subHeight});
        m_nodes[1] = std::make_unique<Quadtree>(m_level + 1,
                                                QRectF{x, y, subWidth, subHeight});
        m_nodes[2] = std::make_unique<Quadtree>(
            m_level + 1, QRectF{x, y + subHeight, subWidth, subHeight});
        m_nodes[3] = std::make_unique<Quadtree>(
            m_level + 1, QRectF{x + subWidth, y + subHeight, subWidth, subHeight});
    }

    // Determine which quadrant the object belongs to
    inline int getIndex(const QRectF &objectBounds) const {
        double verticalMidpoint = m_bounds.x() + (m_bounds.width() / 2);
        double horizontalMidpoint = m_bounds.y() + (m_bounds.height() / 2);

        // Check if object can completely fit within the top or bottom quadrants
        bool topQuadrant = objectBounds.bottom() < horizontalMidpoint;
        bool bottomQuadrant = objectBounds.top() >= horizontalMidpoint;

        // Check if object can completely fit within the left or right quadrants
        bool leftQuadrant = objectBounds.right() < verticalMidpoint;
        bool rightQuadrant = objectBounds.left() >= verticalMidpoint;

        // Object can completely fit within one quadrant
        if (leftQuadrant) {
            if (topQuadrant) {
                return 1;
            } else if (bottomQuadrant) {
                return 2;
            }
        } else if (rightQuadrant) {
            if (topQuadrant) {
                return 0;
            } else if (bottomQuadrant) {
                return 3;
            }
        }

        // Object spans multiple quadrants
        return -1;
    }


    inline int getIndex(float x, float y) const {
        int index = -1;
        double verticalMidpoint = m_bounds.x() + (m_bounds.width() / 2);
        double horizontalMidpoint = m_bounds.y() + (m_bounds.height() / 2);

        // Object can completely fit within the top quadrants
        bool topQuadrant = (y < horizontalMidpoint);

        // Object can completely fit within the bottom quadrants
        bool bottomQuadrant = (y >= horizontalMidpoint);

        // Object can completely fit within the left quadrants
        if (x < verticalMidpoint) {
            if (topQuadrant) {
                index = 1;
            } else if (bottomQuadrant) {
                index = 2;
            }
        }

        // Object can completely fit within the right quadrants
        else if (x >= verticalMidpoint) {
            if (topQuadrant) {
                index = 0;
            } else if (bottomQuadrant) {
                index = 3;
            }
        }

        return index;
    }

    inline bool canCollide(const std::unordered_set<ObjectType>& types1,
                           const std::unordered_set<ObjectType>& types2) const {
        for (const auto& type1 : types1) {
            for (const auto& type2 : types2) {
                if (canCollide(type1, type2))
                    return true;
            }
        }
        return false;
    }

    inline bool canCollide(const ObjectType type1, const ObjectType type2) const {
        auto it = m_collisionMap.find(type1);
        return it != m_collisionMap.end() &&
               it->second.find(type2) != it->second.end();
    }
};
}
}



#endif // QUADTREE_H
