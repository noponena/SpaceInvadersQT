#include "HealthBar.h"
#include <QBrush>

namespace GameObjects {
namespace Ships {

HealthBar::HealthBar(float health, float width, float height, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_currentHealth(health),
    m_maxHealth(health), m_healthPercentage(1.0f),
    m_width(width), m_height(height), m_currentWidth(width)

{}

QRectF HealthBar::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void HealthBar::updateHealth(float amount)
{
    m_currentHealth += amount;
    m_healthPercentage = m_currentHealth / m_maxHealth;
    m_currentWidth = m_width * m_healthPercentage;
}

QBrush HealthBar::selectBrush()
{
    if (m_healthPercentage < 0.3)
        return QBrush(QColor(211, 82, 105));
    if (m_healthPercentage < 0.6)
        return QBrush(QColor(248, 178, 98));

    return QBrush(QColor(23, 184, 144));
}

void HealthBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(QColor(98, 98, 103));
    painter->drawRect(boundingRect());

    painter->setBrush(selectBrush());
    QRectF filledRect(0, 0, m_currentWidth, m_height);
    painter->drawRect(filledRect);
}

} // namespace Ships
} // namespace GameObjects
