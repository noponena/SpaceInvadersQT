#include "EnemyShip.h"
#include <QGraphicsScene>
#include <QPen>
#include <QTimer>

namespace GameObjects {

EnemyShip::EnemyShip(const int maxHp, float speed, int fireRate, const Position &position)
    : Shooter(maxHp, speed, fireRate, position), m_destroyed(false)
{
    int frameWidth = 400;  // width of a single frame
    int frameHeight = 400;  // height of a single frame
    int columns = 4;  // number of columns of frames in the sprite sheet
    int rows = 4;  // number of rows of frames in the sprite sheet
    int targetWidth = 30;  // The width of your ship
    int targetHeight = 30;  // The height of your ship
    QPixmap spriteSheet("C:\\Users\\aaron\\OneDrive\\Tiedostot\\Aaro\\Personal\\Projects\\QT\\SpaceInvaders\\Images\\explosion.png");

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            int x = col * frameWidth;
            int y = row * frameHeight;
            QPixmap frame = spriteSheet.copy(x, y, frameWidth, frameHeight);
            QPixmap scaledFrame = frame.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            m_animationFrames.append(scaledFrame);
        }
    }
    qDebug() << "Total frames:" << m_animationFrames.size();
    connect(this, &EnemyShip::animationCompleted, this, &EnemyShip::onAnimationCompleted);

}

void EnemyShip::initialize()
{
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
    this->calculateBoundingBox();
}

bool EnemyShip::shouldBeDeleted()
{
    return m_destroyed;
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
    if (this->m_currentHp <= 0) {
        m_collidable = false;
        this->playDestructionAnimation();
    }
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
            emit animationCompleted();  // Emit the signal when the animation completes
        }
    });
    animationTimer->start(100);  // Adjust timing to fit your needs
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


void EnemyShip::onAnimationCompleted()
{
    m_destroyed = true;
    qDebug() << "animation completed";
}

} // namespace GameObjects
