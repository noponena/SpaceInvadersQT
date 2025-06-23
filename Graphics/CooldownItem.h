#ifndef COOLDOWNITEM_H
#define COOLDOWNITEM_H

#include <QElapsedTimer>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTimer>

namespace Graphics {

class CooldownItem : public QGraphicsObject {
  Q_OBJECT

public:
  CooldownItem(QPixmap pixmap, QGraphicsItem *parent = nullptr)
      : QGraphicsObject(parent), m_elapsedTime(0), m_cooldownAngle(360),
        m_cooldownInProgress(false), m_updateItervalMs(100),
        m_color(0, 0, 0, 180) {
    m_pixmapItem = new QGraphicsPixmapItem;
    m_pixmapItem->setPixmap(pixmap);

    m_pixmapItem->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, false);

    m_elapsedTimer.start();

    m_cooldownTimer = new QTimer(this);
    connect(m_cooldownTimer, &QTimer::timeout, this,
            &CooldownItem::updateCooldown);
  }

  ~CooldownItem() { delete m_pixmapItem; }

  void startCooldown(int duration) {
    if (!m_cooldownInProgress) {
      m_elapsedTimer.restart();
      m_cooldownInProgress = true;
      m_cooldownDurationMs = duration;
      m_cooldownAngle = 360;
      m_elapsedTime = 0;
      m_cooldownTimer->start(m_updateItervalMs);
      update();
    }
  }

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override {

    painter->setPen(Qt::NoPen);
    m_pixmapItem->paint(painter, option, widget);

    if (m_cooldownInProgress) {
      painter->save();
      painter->setClipRect(m_pixmapItem->boundingRect());
      QRectF bbox = boundingRect();

      qreal radius = qSqrt(qPow(bbox.width(), 2) + qPow(bbox.height(), 2)) / 2;
      QRectF squareBox(bbox.center().x() - radius, bbox.center().y() - radius,
                       2 * radius, 2 * radius);

      qreal proportion =
          static_cast<qreal>(m_elapsedTimer.elapsed()) / m_cooldownDurationMs;
      qreal cooldownAngle = -360.0 * proportion;

      QPainterPath path;
      path.moveTo(bbox.center());
      path.arcTo(squareBox, 90, cooldownAngle);
      path.lineTo(bbox.center());
      path.closeSubpath();

      painter->setBrush(QBrush(m_color));
      painter->drawPath(path);
      painter->restore();
    }
  }

  QRectF boundingRect() const override { return m_pixmapItem->boundingRect(); }

  void setPixmap(const QPixmap &pixmap) { m_pixmapItem->setPixmap(pixmap); }

private slots:
  void updateCooldown() {
    qint64 elapsed = m_elapsedTimer.elapsed();
    float proportion = (m_cooldownDurationMs - elapsed) /
                       static_cast<float>(m_cooldownDurationMs);
    m_cooldownAngle = proportion * 360;
    update();

    if (elapsed >= m_cooldownDurationMs) {
      m_cooldownTimer->stop();
      m_cooldownInProgress = false;
    }
  }

private:
  QGraphicsPixmapItem *m_pixmapItem;
  QTimer *m_cooldownTimer;
  QElapsedTimer m_elapsedTimer;
  int m_cooldownDurationMs;
  int m_elapsedTime;
  float m_cooldownAngle;
  bool m_cooldownInProgress;
  std::uint32_t m_updateItervalMs;
  QColor m_color;
};

} // namespace Graphics
#endif // COOLDOWNITEM_H
