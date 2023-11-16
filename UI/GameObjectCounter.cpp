#include "GameObjectCounter.h"
#include <QFont>

namespace UI {

GameObjectCounter::GameObjectCounter(QGraphicsItem *parent)
    : QGraphicsTextItem(parent) {
  setDefaultTextColor(Qt::white);
  setFont(QFont("times", 12));
}

void GameObjectCounter::updateObjectCount(const int amount) {
  m_objectCount += amount;
  setPlainText(QString("Object count: %1").arg(m_objectCount));
}

void GameObjectCounter::setObjectCount(const int count) {
  m_objectCount = count;
  setPlainText(QString("Object count: %1").arg(m_objectCount));
}
} // namespace UI
