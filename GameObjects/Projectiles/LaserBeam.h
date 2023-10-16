#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class LaserBeam : public Projectile, public QGraphicsRectItem {
public:
  LaserBeam(Position position, float speed, Qt::GlobalColor color);

private:
  Qt::GlobalColor m_color;

  // GameObject interface
protected:
  void initializeGraphicsItem() override;

  // QGraphicsItem interface
public:
  QRectF boundingRect() const override;

  // GameObject interface
protected:
  QRectF getNonTransparentBoundingRect() override;

  // GameObject interface
protected:
  QPointF getPixmapScaledSize() const override;
  QString getPixmapResourcePath() const override;
  QPixmap getPixmap() const override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // LASERBEAM_H
