#ifndef GAMEOBJECTS_SHIPS_HEALTHBAR_H
#define GAMEOBJECTS_SHIPS_HEALTHBAR_H

#include <QGraphicsItem>
#include <QPainter>

namespace GameObjects {
namespace Ships {

class HealthBar : public QGraphicsItem {
public:
  HealthBar(float health, float width, float height,
            QGraphicsItem *parent = nullptr);
  QRectF boundingRect() const override;
  void updateHealth(float amount);

private:
  float m_currentHealth;
  float m_maxHealth;
  float m_healthPercentage;
  float m_width;
  float m_height;
  float m_currentWidth;

  QBrush selectBrush();
  void clampCurrentHealth();

  // QGraphicsItem interface
public:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;
};

} // namespace Ships
} // namespace GameObjects

#endif // GAMEOBJECTS_SHIPS_HEALTHBAR_H
