#include "TextureRegistry.h"

namespace Graphics {

TextureRegistry::~TextureRegistry() { clear(); }

void TextureRegistry::clear() {
  if (m_gl) {
    for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
      if (it.value().handle)
        m_gl->glDeleteTextures(1, &it.value().handle);
    }
    if (m_placeholder.handle)
      m_gl->glDeleteTextures(1, &m_placeholder.handle);
  }
  m_textures.clear();
  m_placeholder = TextureInfo{0, 0, 0};
}

} // namespace Graphics
