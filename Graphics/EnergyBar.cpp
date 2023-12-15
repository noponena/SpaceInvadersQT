#include "EnergyBar.h"

namespace Graphics {

EnergyBar::EnergyBar(float maxEnergy, float width, float height,
                     QGraphicsItem *parent)
    : ProgressBar(maxEnergy, width, height, parent) {}

QBrush EnergyBar::selectBrush() { return QBrush(QColor(86, 123, 179)); }

} // namespace Graphics
