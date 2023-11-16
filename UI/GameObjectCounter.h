#ifndef UI_GAMEOBJECTCOUNTER_H
#define UI_GAMEOBJECTCOUNTER_H

#include <QGraphicsTextItem>

namespace UI {

class GameObjectCounter : public QGraphicsTextItem {
  Q_OBJECT
public:
  explicit GameObjectCounter(QGraphicsItem *parent = nullptr);

public:
  void updateObjectCount(const int amount);
  void setObjectCount(const int count);

private:
  int m_objectCount = 0;
};

} // namespace UI

#endif // UI_GAMEOBJECTCOUNTER_H
