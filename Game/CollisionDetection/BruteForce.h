#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include "GameObjects/GameObject.h"
#include <vector>

namespace Game {
namespace CollisionDetection {

class BruteForce {

public:
  void detect(const std::vector<std::shared_ptr<GameObjects::GameObject>>
                  &gameObjects) {
    for (auto it1 = gameObjects.begin(); it1 != gameObjects.end(); ++it1) {
      for (auto it2 = std::next(it1); it2 != gameObjects.end(); ++it2) {
        if (canCollide((*it1)->objectTypes(), (**it2).objectTypes())) {
          if ((*it1)->isCollidingWith(**it2)) {
            (*it1)->collide(**it2);
          }
        }
      }
    }
  }

private:
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
};

} // namespace CollisionDetection
} // namespace Game

#endif // BRUTEFORCE_H
