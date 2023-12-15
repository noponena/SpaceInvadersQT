#include "EnergyBar.h"

namespace Graphics {

EnergyBar::EnergyBar(float maxEnergy, float width, float height,
                     bool showNumericProgression, QGraphicsItem *parent)
    : ProgressBar(maxEnergy, width, height, showNumericProgression, parent) {}

QBrush EnergyBar::selectBrush() { return QBrush(QColor(86, 123, 179)); }

} // namespace Graphics
