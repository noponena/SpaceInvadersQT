#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsView>
#include <QKeyEvent>
#include "GameState.h"

namespace Game {
class GameRunner : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameRunner(QWidget *parent = nullptr);
    void startGame();

    QGraphicsScene *scene();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
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
    void render();
    void updateFps();

    void initializeGameObjects();

signals:
    void fpsUpdated(int fps);

public slots:
    void onLaserAdded(const std::shared_ptr<GameObjects::Laser>& laser) {
        m_scene.addItem(laser->graphicsItem());
    }
    void onEnemyAdded(const std::shared_ptr<GameObjects::EnemyShip>& enemy) {
        m_scene.addItem(enemy->graphicsItem());
    }
    void onObjectDeleted(const std::shared_ptr<GameObjects::GameObject>& object) {
        m_scene.removeItem(object->graphicsItem());
        qDebug() << "object deleted";
    }

};
}



#endif // GAMERUNNER_H
