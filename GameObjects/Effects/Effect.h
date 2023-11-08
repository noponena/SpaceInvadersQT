#ifndef EFFECT_H
#define EFFECT_H

#include <QGraphicsItem>

namespace GameObjects {
namespace Effects {

class Effect : public QObject, public QGraphicsItem {
public:
  Effect() : m_position(){};

  virtual void setPosition(QPointF newPosition);

protected:
  QPointF m_position;
};

inline void Effect::setPosition(QPointF newPosition)
{
    m_position = newPosition;
}

} // namespace Effects
} // namespace GameObjects

#endif // EFFECT_H
