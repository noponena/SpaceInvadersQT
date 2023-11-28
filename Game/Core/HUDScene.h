#ifndef HUDSCENE_H
#define HUDSCENE_H


#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

namespace Game {
namespace Core {
class HUDScene : public QGraphicsView {
    Q_OBJECT

public:
    HUDScene(QWidget *parent = nullptr);

protected:
    // Override keyPressEvent to handle weapon selection
    void keyPressEvent(QKeyEvent *event) override;

private:
    QGraphicsScene m_scene;
    QGraphicsPixmapItem *weaponIcons[4];

    void selectWeapon(int weaponIndex) {
        // Placeholder for weapon selection logic
        // You can add visual effects or logic to update game state here
        for (int i = 0; i < 4; ++i) {
            if (i == weaponIndex) {
                // Apply a visual effect to indicate selection, e.g., a border
                weaponIcons[i]->setOpacity(1.0); // Full opacity for selected weapon
            } else {
                weaponIcons[i]->setOpacity(0.5); // Reduced opacity for non-selected weapons
            }
        }
    }
};
}
}


#endif // HUDSCENE_H
