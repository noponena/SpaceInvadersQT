#pragma once

#include <QOpenGLFunctions_3_3_Core>

namespace UI {
class IRenderable {
public:
  virtual ~IRenderable() {}
  virtual void render(QOpenGLFunctions_3_3_Core *gl, float screenWidth,
                      float screenHeight, float x = 0.f, float y = 0.f) = 0;
};
} // namespace UI
