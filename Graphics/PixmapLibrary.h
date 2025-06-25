#ifndef PIXMAPLIBRARY_H
#define PIXMAPLIBRARY_H

#include <QMap>
#include <QPixmap>
#include <QPointF>
#include <QString>

namespace Graphics {

class PixmapLibrary {
public:
  static QPixmap &getPixmap(const QString &path, float width, float height,
                            bool keepAspectRatio = true) {
    static QMap<QString, QPixmap> pixmaps;
    QString key = generateKey(path, width, height);

    if (!pixmaps.contains(key)) {
      QPixmap pixmap;
      pixmap.load(path);
      pixmap = setPixmapSize(pixmap, width, height, keepAspectRatio);
      pixmaps.insert(key, pixmap);
    }
    return pixmaps[key];
  }

  static void
  preloadPixmaps(const QList<std::tuple<QString, float, float, bool>> &list) {
    for (const auto &tup : list) {
      getPixmap(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                std::get<3>(tup));
    }
  }

private:
  inline static QPixmap setPixmapSize(QPixmap &pixmap, float width,
                                      float height,
                                      bool keepAspectRatio = true) {
    Qt::AspectRatioMode mode =
        keepAspectRatio ? Qt::KeepAspectRatio : Qt::IgnoreAspectRatio;
    pixmap = pixmap.scaled(width, height, mode, Qt::SmoothTransformation);
    return pixmap;
  }
  inline static QString generateKey(const QString &path, float scaleX,
                                    float scaleY) {
    return QString("%1_%2_%3").arg(path).arg(scaleX).arg(scaleY);
  }
};

} // namespace Graphics
#endif // PIXMAPLIBRARY_H
