#include "Ship.h"

namespace GameObjects {

Ship::Ship(const int maxHp, float speed, const Position &position)
    : GameObject(position, speed), m_maxHp(maxHp), m_speed(speed), m_destroyed(false)
{
    m_currentHp = maxHp;
}

void Ship::takeDamage(int amount)
{
    m_currentHp -= amount;
    if (m_currentHp < 0) {
        m_currentHp = 0;
        this->die();
    }
}

void Ship::heal(int amount)
{
    if (this->isAlive())
    {
        m_currentHp += amount;
        if (m_currentHp > m_maxHp) {
            m_currentHp = m_maxHp;
        }
    }
}

bool Ship::isAlive()
{
    return m_currentHp > 0;
}

void Ship::initializeDestructionAnimation()
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
    connect(this, &Ship::animationCompleted, this, &Ship::onAnimationCompleted);
}

void Ship::die()
{
    //this->update(16);
    m_collidable = false;
    this->setXMovementFunc(nullptr);
    this->setYMovementFunc(nullptr);
    this->playDestructionAnimation();
}

void Ship::onAnimationCompleted()
{
    m_destroyed = true;
    qDebug() << "animation completed";
}

void Ship::initialize()
{

}

bool Ship::shouldBeDeleted()
{
    return m_destroyed;
}


} // namespace GameObjects
