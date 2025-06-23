#ifndef GAME_COLLISIONDETECTION_BVHTREE_H
#define GAME_COLLISIONDETECTION_BVHTREE_H

#include "GameObjects/GameObject.h"
#include <set>
#include <unordered_map>
#include <vector>

namespace Game {
namespace CollisionDetection {

struct BVHNode {
  QRectF bbox;
  std::shared_ptr<BVHNode> leftChild;
  std::shared_ptr<BVHNode> rightChild;
  std::weak_ptr<BVHNode> parent;
  std::vector<GameObjects::GameObject *> objects; // Leaf nodes hold game
                                                 // objects

  BVHNode() = default;
};

class BVHTree {
public:
  std::shared_ptr<BVHNode> root;

  BVHTree();
  ~BVHTree();
  std::shared_ptr<BVHNode>
  build(std::vector<GameObjects::GameObject *> &objects);
  void insert(GameObjects::GameObject *object);
  void remove(GameObjects::GameObject *object);
  void update(GameObjects::GameObject *object);
  std::vector<GameObjects::GameObject *>
  query(std::shared_ptr<BVHNode> node, GameObjects::GameObject *queryObject);
  void clearProcessedPairs();

private:
  inline QRectF
  calculateBoundingBox(const std::vector<GameObjects::GameObject *> &objects);
  inline std::pair<std::vector<GameObjects::GameObject *>,
                   std::vector<GameObjects::GameObject *>>
  splitObjects(std::vector<GameObjects::GameObject *> &objects, bool splitX);
  inline QRectF combineBoundingBoxes(const QRectF &bbox1, const QRectF &bbox2);

  bool m_splitX;
  int m_intersectionCounter;
  std::set<std::pair<std::uint64_t, std::uint64_t>> m_processedPairs;

  std::vector<GameObjects::GameObject *> m_objects;
  std::unordered_map<GameObjects::GameObject *, std::shared_ptr<BVHNode>>
      m_leafMap;
  using ObjectType = GameObjects::ObjectType;
  std::unordered_map<GameObjects::ObjectType,
                     std::unordered_set<GameObjects::ObjectType>>
      m_collisionMap{
          {ObjectType::PLAYER_SHIP,
           {ObjectType::ENEMY_PROJECTILE, ObjectType::COLLECTABLE}},

          {ObjectType::ENEMY_SHIP, {ObjectType::PLAYER_PROJECTILE}},
      };

  inline bool canCollide(const std::unordered_set<ObjectType> &types1,
                         const std::unordered_set<ObjectType> &types2) const {
    for (const auto &type1 : types1) {
      for (const auto &type2 : types2) {
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

  inline bool
  shouldSplitX(const std::vector<GameObjects::GameObject *> &objects);
};

} // namespace CollisionDetection
} // namespace Game

#endif // GAME_COLLISIONDETECTION_BVHTREE_H
