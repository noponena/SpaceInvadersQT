#include "FPSCounter.h"
#include <QFont>

namespace UI {

FPSCounter::FPSCounter(QGraphicsItem *parent) : QGraphicsTextItem(parent) {
  setDefaultTextColor(Qt::white);
  setFont(QFont("times", 12));
}

void FPSCounter::updateFPS(int fps) {
  setPlainText(QString("FPS: %1").arg(fps));
}

} // namespace UI
