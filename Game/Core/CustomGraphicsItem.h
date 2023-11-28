#ifndef CUSTOMGRAPHICSITEM_H
#define CUSTOMGRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include <QPainter>

class CustomGraphicsItem : public QGraphicsPixmapItem {
  // ... other members and methods ...

public:
  CustomGraphicsItem(const QPixmap &pixmap) : QGraphicsPixmapItem(pixmap) {}

  // QGraphicsItem interface
public:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override {
    // Call the base class paint method to draw the item itself
    QGraphicsPixmapItem::paint(painter, option, widget);

    // Save the painter's state
    painter->save();

    // Set the pen for drawing the bounding box outline
    QPen pen(Qt::red, 1, Qt::DashLine);
    painter->setPen(pen);

    // Set the brush to NoBrush to make the rectangle non-solid
    painter->setBrush(Qt::NoBrush);

    // Draw the bounding box outline
    painter->drawRect(boundingRect());

    // Restore the painter's state
    painter->restore();
  }
};

#endif // CUSTOMGRAPHICSITEM_H
