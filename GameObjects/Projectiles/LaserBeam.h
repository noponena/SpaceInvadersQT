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
  static const QPixmap& getPixmap() {
      static QPixmap sharedPixmap(":/Images/player_laser_projectile.png");
      qreal width = 29.055;
      qreal height = 30.0;
      sharedPixmap = sharedPixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      return sharedPixmap;
  }

  // GameObject interface
protected:
  void initializeGraphicsItem() override;

  // QGraphicsItem interface
public:
  QRectF boundingRect() const override;

  // GameObject interface
protected:
  QRectF getNonTransparentBoundingRect() override;
};
} // namespace Projectiles

} // namespace GameObjects

#endif // LASERBEAM_H
