#ifndef LASERBEAM_H
#define LASERBEAM_H

#include "Projectile.h"

namespace GameObjects {
namespace Projectiles {
class LaserBeam : public Projectile, public QGraphicsRectItem {
public:
  LaserBeam(Position position, float speed, Qt::GlobalColor color);

  // QGraphicsItem interface
public:
  QRectF boundingRect() const override;

  // GameObject interface
protected:
  QRectF getNonTransparentBoundingRect() override;
  QPointF getPixmapScaledSize() const override;
  QString getPixmapResourcePath() const override;
  QPixmap getPixmap() const override;

private:
  Qt::GlobalColor m_color;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // LASERBEAM_H
