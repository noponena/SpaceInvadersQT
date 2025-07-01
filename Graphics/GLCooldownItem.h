// GLCooldownItem.h
#pragma once
#include <GL/gl.h>
#include <chrono>
#include <string>

class GLCooldownItem {
public:
  GLCooldownItem();
  void setTexture(const std::string &imagePath); // Load OpenGL texture
  void setPosition(float x, float y);
  void startCooldown(uint32_t ms);
  void render();

private:
  float m_x, m_y;
  GLuint m_texture;
  uint32_t m_cooldownMs = 0;
  std::chrono::steady_clock::time_point m_cooldownStart;
  // For cooldown arc: use a simple triangle fan with alpha mask or shader
};
