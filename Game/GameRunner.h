#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#include <QElapsedTimer>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include "GameState.h"
#include "LevelManager.h"

namespace Game {
class GameRunner : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameRunner(QWidget *parent = nullptr);
    void startGame();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override {event->ignore();}

private:
    std::unique_ptr<LevelManager> m_levelManager;
    std::shared_ptr<GameObjects::PlayerShip> m_playerShip;
    QElapsedTimer m_elapsedTimer;
    QElapsedTimer m_fpsTimer;
    Game::GameState m_gameState;
    QSet<int> m_pressedKeys;
    QGraphicsScene m_scene;
    int m_frameCount = 0;

    void setupView();
    void setupCounters();
    void setupConnections();
    void gameLoop();

    inline void processInput(float deltaTime);
    inline void updateGameState(float deltaTime);
    inline void updateFps();
    inline void detectCollisions();

    const std::list<std::shared_ptr<GameObjects::GameObject>>* m_gameObjects;

    using Action = std::function<void(float)>;
    const std::unordered_map<int, Action> m_keyActions
    {
        { Qt::Key_Left,   [&](float dt) { m_playerShip->accelerateLeft(dt);           } },
        { Qt::Key_Right,  [&](float dt) { m_playerShip->accelerateRight(dt);          } },
        { Qt::Key_Down,   [&](float dt) { m_playerShip->accelerateDown(dt);           } },
        { Qt::Key_Up,     [&](float dt) { m_playerShip->accelerateUp(dt);             } },
        { Qt::Key_Space,  [&](float dt) { Q_UNUSED(dt); m_playerShip->shoot();        } },
        { Qt::Key_Q,      [&](float dt) { Q_UNUSED(dt); m_gameState.initEnemyShips(); } },
        { Qt::Key_U,  [&](float dt) { Q_UNUSED(dt); m_playerShip->updateFireRate();   } },
        { Qt::Key_D,  [&](float dt) { Q_UNUSED(dt);m_playerShip->updateFireRate(-1);  } },
    };

signals:
    void fpsUpdated(int fps);

public slots:
    void onObjectAdded(const std::shared_ptr<GameObjects::GameObject>& object) {
        m_scene.addItem(object->graphicsItem());
    }
    void onObjectDeleted(const std::shared_ptr<GameObjects::GameObject>& object) {
        m_scene.removeItem(object->graphicsItem());
    }
    void onCollisionDetected(const std::shared_ptr<GameObjects::GameObject>& obj1,
                             const std::shared_ptr<GameObjects::GameObject>& obj2) {
        obj1->doCollide(*obj2);
    }
};
}

#endif // GAMERUNNER_H
