#ifndef GRAPHICS_ENERGYBAR_H
#define GRAPHICS_ENERGYBAR_H

#include "ProgressBar.h"

namespace Graphics {

class EnergyBar : public ProgressBar {
public:
  EnergyBar(float maxEnergy, float width, float height,
            bool showNumericProgression = false,
            QGraphicsItem *parent = nullptr);

  // ProgressBar interface
protected:
  QBrush selectBrush() override;
};

} // namespace Graphics

#endif // GRAPHICS_ENERGYBAR_H
