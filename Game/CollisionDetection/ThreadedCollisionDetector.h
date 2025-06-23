#ifndef THREADEDCOLLISIONDETECTOR_H
#define THREADEDCOLLISIONDETECTOR_H

#include "GameObjects/GameObject.h"
#include <QObject>
#include <QTimer>
#include <QVector>
#include <mutex>

namespace Game {
class ThreadedCollisionDetector : public QObject {
  Q_OBJECT

public:
  explicit ThreadedCollisionDetector(
      const std::vector<std::shared_ptr<GameObjects::GameObject>> &gameObjects,
      std::mutex &mutex)
      : m_mutex(mutex), m_gameObjects(gameObjects) {

    m_data.clear();
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this,
            &ThreadedCollisionDetector::handleCollisions);
    m_timer->start(0);
  }

public slots:

  void onDataReadyForCollisionCheck(
      std::unordered_map<
          std::uint64_t,
          std::pair<QRectF, std::unordered_set<GameObjects::ObjectType>>>
          data) {
    // handleCollisions();
    m_data = data;
  }

private slots:
  void handleCollisions() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_data.empty()) {
      std::vector<std::pair<std::uint64_t, std::uint64_t>> collidingPairs;
      for (auto it1 = m_data.begin(); it1 != m_data.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != m_data.end(); ++it2) {
          if (canCollide(it1->second.second, it2->second.second)) {
            if (it1->second.first.intersects(it2->second.first)) {
              collidingPairs.push_back({it1->first, it2->first});
            }
          }
        }
      }

      if (!collidingPairs.empty())
        emit collisionsDetected(collidingPairs);

      m_data.clear();
    }
  }

signals:

  void collisionsDetected(
      std::vector<std::pair<std::uint64_t, std::uint64_t>> collidingPairs);

private:
  std::unordered_map<
      std::uint64_t,
      std::pair<QRectF, std::unordered_set<GameObjects::ObjectType>>>
      m_data;
  QTimer *m_timer;
  std::mutex &m_mutex;
  const std::vector<std::shared_ptr<GameObjects::GameObject>> &m_gameObjects;

  using ObjectType = GameObjects::ObjectType;
  std::unordered_map<ObjectType, std::unordered_set<ObjectType>> m_collisionMap{
      {ObjectType::PLAYER_SHIP,
       {ObjectType::ENEMY_PROJECTILE, ObjectType::COLLECTABLE}},

      {ObjectType::ENEMY_SHIP, {ObjectType::PLAYER_PROJECTILE}},
  };

  std::map<ObjectType, std::string> m_strMap{
      {ObjectType::PLAYER_SHIP, "PLAYER_SHIP"},
      {ObjectType::ENEMY_PROJECTILE, "ENEMY_PROJECTILE"},
      {ObjectType::COLLECTABLE, "COLLECTABLE"},
      {ObjectType::ENEMY_SHIP, "ENEMY_SHIP"},
      {ObjectType::PLAYER_PROJECTILE, "PLAYER_PROJECTILE"},
  };

  inline bool canCollide(const std::unordered_set<ObjectType> &types1,
                         const std::unordered_set<ObjectType> &types2) {
    for (const auto &type1 : types1) {
      for (const auto &type2 : types2) {
        if (canCollide(type1, type2))
          return true;
      }
    }
    return false;
  }

  inline bool canCollide(const ObjectType type1, const ObjectType type2) {
    // qDebug() << "checking if type" << m_strMap[type1] << "can collide with"
    // << m_strMap[type2];
    auto it = m_collisionMap.find(type1);
    return it != m_collisionMap.end() &&
           it->second.find(type2) != it->second.end();
  }
};

} // namespace Game

#endif // THREADEDCOLLISIONDETECTOR_H
