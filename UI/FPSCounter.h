#ifndef UI_FPSCOUNTER_H
#define UI_FPSCOUNTER_H

#include <QGraphicsTextItem>

namespace UI {

class FPSCounter : public QGraphicsTextItem {
  Q_OBJECT

public:
  explicit FPSCounter(QGraphicsItem *parent = nullptr);

public slots:
  void updateFPS(int fps);
};

} // namespace UI

#endif // UI_FPSCOUNTER_H
