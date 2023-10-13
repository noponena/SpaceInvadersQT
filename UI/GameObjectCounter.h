#ifndef UI_GAMEOBJECTCOUNTER_H
#define UI_GAMEOBJECTCOUNTER_H

#include <QGraphicsTextItem>

namespace UI {

class GameObjectCounter : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit GameObjectCounter(QGraphicsItem *parent = nullptr);
public slots:
    void updateObjectCount(int amount);
private:
    int m_objectCount = 0;
};

} // namespace UI

#endif // UI_GAMEOBJECTCOUNTER_H
