#ifndef GRAPHICS_HEALTHBAR_H
#define GRAPHICS_HEALTHBAR_H

#include "ProgressBar.h"

namespace Graphics {

class HealthBar : public ProgressBar {
public:
  HealthBar(float maxHealth, float width, float height,
            bool showNumericProgression = false);

  // ProgressBar interface
private:
  QBrush selectBrush() override;
};

} // namespace Graphics

#endif // GRAPHICS_HEALTHBAR_H
