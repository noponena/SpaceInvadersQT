#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include "GameState.h"

namespace Game {
class GameRunner : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameRunner(QWidget *parent = nullptr);
    ~GameRunner();
    void startGame();

    QGraphicsScene *scene();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        Q_UNUSED(event);
    }

    void wheelEvent(QWheelEvent *event) override
    {
        event->ignore();
    }
private:
    QElapsedTimer m_elapsedTimer;
    int m_frameCount = 0;
    QElapsedTimer m_fpsTimer;
    Game::GameState m_gameState;
    QSet<int> m_pressedKeys;
    QGraphicsScene m_scene;

    void gameLoop();
    void processInput(qint64 deltaTime);
    void updateGameState(qint64 deltaTime);
    void updateFps();
    void initializeCollisionDetection();

    void initializeGameObjects();
    bool m_perfTest = false;

    QThread* m_collisionThread;

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
