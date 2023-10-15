#ifndef EFFECT_H
#define EFFECT_H

#include <QGraphicsItem>

namespace GameObjects {
namespace Effects {


class Effect: public QObject, public QGraphicsItem {
public:
    Effect(QPointF position)
        :m_position(position) {};
protected:
    QPointF m_position;
};

}
}

#endif // EFFECT_H
