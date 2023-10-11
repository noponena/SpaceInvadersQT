#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <QObject>
#include <QVector>
#include <mutex>
#include <QTimer>
#include "GameObjects/GameObject.h"

namespace Game {
class CollisionDetector : public QObject {
    Q_OBJECT

public:
    explicit CollisionDetector(const std::list<std::shared_ptr<GameObjects::GameObject>>& gameObjects, std::mutex& mutex)
        : m_mutex(mutex), m_gameObjects(gameObjects) {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &CollisionDetector::checkCollisions);
        m_timer->start(8);  // Set the interval as needed, e.g., 16ms for ~60 checks per second
    }

public slots:
    void checkCollisions() {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto it1 = m_gameObjects.begin(); it1 != m_gameObjects.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != m_gameObjects.end(); ++it2) {
                if ((*it1)->isCollision(**it2)) {
                    emit collisionDetected(*it1, *it2);
                }
            }
        }
    }

signals:
    void collisionDetected(const std::shared_ptr<GameObjects::GameObject>& object1,
                           const std::shared_ptr<GameObjects::GameObject>& object2);

private:
    QTimer* m_timer;
    std::mutex& m_mutex;
    const std::list<std::shared_ptr<GameObjects::GameObject>>& m_gameObjects;
};

}



#endif // COLLISIONDETECTOR_H
