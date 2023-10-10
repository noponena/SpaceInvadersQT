#include "EnemyShip.h"
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>

namespace GameObjects {

EnemyShip::EnemyShip(const int maxHp, float speed, int fireRate, const Position &position)
    : Shooter(maxHp, speed, fireRate, position)
{}

void EnemyShip::initialize()
{
//    this->setMovementFunc(
//        [this](const std::tuple<int, int>& currentPosition, int deltaTime) {
//            int originalX = std::get<0>(currentPosition);
//            int originalY = std::get<1>(currentPosition);

//            // Horizontal oscillation
//            //float timeInSeconds = deltaTime / 1000.0f;  // Assuming deltaTime is in milliseconds
//            float timeInSeconds = 0.016;
//            //qDebug() << "timeInSeconds:" << timeInSeconds;
//            float value = m_oscillationFrequency * timeInSeconds * originalY;
//            //qDebug() << "value:" << value;
//            int deltaX = static_cast<int>(10 * std::sin(value));

//            // Constant vertical motion
//            int deltaY = m_speed * deltaTime;

//            int x = originalX + deltaX;
//            int y = originalY + deltaY;
//            return std::make_tuple(x, y);
//        }
//    );

    this->initializeDestructionAnimation();
    QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem();

    // Set the color and pen properties
    polygonItem->setBrush(Qt::green);
    polygonItem->setPen(QPen(Qt::black));

    // Create a QPolygon to represent the triangle and set it to the QGraphicsPolygonItem
    QPolygon triangle;
    triangle << QPoint(0, 10) << QPoint(-10, -20) << QPoint(10, -20);  // Assuming the ship is centered at (0, 0)
    polygonItem->setPolygon(triangle);

    // Assign the polygonItem to m_graphicsItem
    m_graphicsItem = polygonItem;
    this->updateBoundingBox();
}

void EnemyShip::shoot()
{

}

void EnemyShip::collideWith(GameObject &other) {
    other.collideWithEnemyShip(*this);
    qDebug() << "enemy ship collided!";
}

void EnemyShip::collideWithProjectile(Projectile &projectile)
{
    int damageValue = projectile.getDamage();  // Assume Projectile has a method damage()
    this->takeDamage(damageValue);
    qDebug() << "Ouch! HP left:" << this->m_currentHp;
}

void EnemyShip::collideWithEnemyShip(EnemyShip &enemyShip)
{
    this->takeDamage(10000);
}

void EnemyShip::playDestructionAnimation() {
    qDebug() << "playing destruction animation";
    this->switchToPixmapItem();  // Switch to using a QGraphicsPixmapItem
    this->moveX(-15);
    this->moveY(-20);
    m_frameIndex = 0;  // Reset frameIndex
    QTimer *animationTimer = new QTimer();
    connect(animationTimer, &QTimer::timeout, this, [this, animationTimer]() {
        QGraphicsPixmapItem *pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(m_graphicsItem);
        if(pixmapItem && m_frameIndex < m_animationFrames.size()) {
            pixmapItem->setPixmap(m_animationFrames[m_frameIndex]);
            m_frameIndex++;
        } else {
            animationTimer->stop();
            animationTimer->deleteLater();
            emit animationCompleted();
        }
    });
    animationTimer->start(100);
}



void EnemyShip::switchToPixmapItem() {
    if (m_graphicsItem) {
        // Get the current position of the polygon item
        QPointF position(500, 500);// = m_graphicsItem->scenePos();

        // Create a new QGraphicsPixmapItem
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem();

        // Set the position of the new pixmap item to match the old polygon item
        pixmapItem->setPos(position);

        // Replace the old m_graphicsItem with the new pixmap item
        QGraphicsScene* scene = m_graphicsItem->scene();
        scene->removeItem(m_graphicsItem);
        delete m_graphicsItem;
        m_graphicsItem = pixmapItem;
        scene->addItem(m_graphicsItem);
        pixmapItem->setZValue(1000);
        pixmapItem->setTransformOriginPoint(pixmapItem->boundingRect().center());
    }
}

bool EnemyShip::shouldBeDeleted()
{
    return m_destroyed || m_position.isBeyondScreenBottomLimit(30);
}

} // namespace GameObjects
