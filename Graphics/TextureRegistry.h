#ifndef TEXTUREREGISTRY_H
#define TEXTUREREGISTRY_H

#include <GL/gl.h>
#include <QDir>
#include <QHash>
#include <QImage>
#include <QOpenGLFunctions_3_3_Core>
#include <QString>

namespace Graphics {

struct TextureInfo {
  GLuint handle;
  int width;
  int height;
};

class TextureRegistry {
public:
  static TextureRegistry &instance() {
    static TextureRegistry reg;
    return reg;
  }

  void setGlContext(QOpenGLFunctions_3_3_Core *gl) {
    m_gl = gl;
    createPlaceholder(); // ensure placeholder is created once gl is set!
  }

  const TextureInfo &getOrCreateTexture(const QString &path) {
    if (!m_gl) {
      throw std::invalid_argument("GL context has not been set!");
    }

    if (m_textures.contains(path))
      return m_textures[path];

    QImage img(path);
    if (img.isNull()) {
      // Log and return placeholder
      qWarning() << "[TextureRegistry] Could not load" << path
                 << ", using placeholder.";
      return m_placeholder;
    }

    if (img.format() != QImage::Format_RGBA8888)
      img = img.convertToFormat(QImage::Format_RGBA8888);
    GLuint tex;
    m_gl->glGenTextures(1, &tex);
    m_gl->glBindTexture(GL_TEXTURE_2D, tex);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl->glBindTexture(GL_TEXTURE_2D, 0);
    m_textures[path] = TextureInfo{tex, img.width(), img.height()};
    qDebug() << "[TextureRegistry] Created a texture: path=" << path << " width=" << img.width()
             << " height=" << img.height();
    return m_textures[path];
  }

  void preloadAllFromDir(const QString &dirPath) {
    if (!m_gl) {
      throw std::invalid_argument("GL context has not been set!");
    }
    QDir dir(dirPath);
    QStringList pngs = dir.entryList(QStringList() << "*.png", QDir::Files);
    for (int i = 0; i < pngs.size(); ++i) {
      QString fullPath = dir.filePath(pngs.at(i));
      if (!fullPath.startsWith(":/"))
        fullPath = ":/" + fullPath;
      getOrCreateTexture(fullPath);
    }
  }

private:
  TextureRegistry() = default;
  // Prevent copying
  TextureRegistry(const TextureRegistry &) = delete;
  TextureRegistry &operator=(const TextureRegistry &) = delete;

  QOpenGLFunctions_3_3_Core *m_gl = nullptr;
  QHash<QString, TextureInfo> m_textures;
  TextureInfo m_placeholder;

  void createPlaceholder() {
    if (!m_gl)
      return;
    // Only create placeholder once
    if (m_placeholder.handle)
      return;
    QImage placeholder(32, 32, QImage::Format_RGBA8888);
    placeholder.fill(Qt::magenta);
    // Checkerboard overlay
    for (int y = 0; y < 32; ++y)
      for (int x = 0; x < 32; ++x)
        if ((x / 8 + y / 8) % 2)
          placeholder.setPixelColor(x, y, Qt::black);

    m_gl->glGenTextures(1, &m_placeholder.handle);
    m_gl->glBindTexture(GL_TEXTURE_2D, m_placeholder.handle);
    m_gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA,
                       GL_UNSIGNED_BYTE, placeholder.bits());
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gl->glBindTexture(GL_TEXTURE_2D, 0);
    m_placeholder.width = 32;
    m_placeholder.height = 32;
  }
};

} // namespace Graphics
#endif // TEXTUREREGISTRY_H
