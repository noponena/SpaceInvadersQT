#include "HUDScene.h"
#include "Graphics/PixmapLibrary.h"

Game::Core::HUDScene::HUDScene(QWidget *parent) : QGraphicsView(parent), m_scene(this)
{

    QPixmap weapon1 = Graphics::PixmapLibrary::getPixmap(
        ":/Images/black_hole.png", 25, 25);
    QPixmap weapon2 = Graphics::PixmapLibrary::getPixmap(
        ":/Images/wave.png", 25, 25);
    QPixmap weapon3 = Graphics::PixmapLibrary::getPixmap(
        ":/Images/placeholder.png", 25, 25);
    QPixmap weapon4 = Graphics::PixmapLibrary::getPixmap(
        ":/Images/placeholder.png", 25, 25);

    // Create QGraphicsPixmapItem for each weapon and add to the scene
    weaponIcons[0] = m_scene.addPixmap(weapon1);
    weaponIcons[1] = m_scene.addPixmap(weapon2);
    weaponIcons[2] = m_scene.addPixmap(weapon3);
    weaponIcons[3] = m_scene.addPixmap(weapon4);

    // Position the weapon icons within the scene
    for (int i = 0; i < 4; ++i) {
        weaponIcons[i]->setPos(i * 50, 0); // Adjust positioning as needed
    }
}

void Game::Core::HUDScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_1:
        selectWeapon(0);
        break;
    case Qt::Key_2:
        selectWeapon(1);
        break;
    case Qt::Key_3:
        selectWeapon(2);
        break;
    case Qt::Key_4:
        selectWeapon(3);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
