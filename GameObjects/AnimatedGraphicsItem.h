#ifndef ANIMATEDGRAPHICSITEM_H
#define ANIMATEDGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsItem>

class AnimatedGraphicsItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
private:
    QGraphicsItem* m_item;
public:
    AnimatedGraphicsItem(QGraphicsItem* item) : m_item(item) {}
    QPointF pos() const { return m_item->pos(); }
    void setPos(const QPointF& pos) { m_item->setPos(pos); }
};


#endif // ANIMATEDGRAPHICSITEM_H
