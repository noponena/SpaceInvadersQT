#include <QSize>
#include <QVector2D>

namespace Graphics {
namespace Animations {

struct AnimationInfo {
  QSize sheetSize;
  int columns;
  int rows;
  std::vector<std::pair<QVector2D, QVector2D>> frameUVs;
  int frameDurationMs = 100;
  int frameCount() const { return frameUVs.size(); }
};

class AnimationPlayer {
public:
  AnimationPlayer()
      : m_currentFrame(0), m_timeAccumulatorMs(0), m_playing(false),
        m_currentAnimationFinished(false), m_loop(false) {}

  void update(int deltaMs) {
    if (!m_playing || m_animation.frameCount() == 0 ||
        m_currentAnimationFinished)
      return;
    m_timeAccumulatorMs += deltaMs;

    while (m_timeAccumulatorMs >= m_animation.frameDurationMs) {
      m_timeAccumulatorMs -= m_animation.frameDurationMs;
      if (m_currentFrame + 1 < m_animation.frameCount()) {
        m_currentFrame++;
      } else {
        if (m_loop) {
          m_currentFrame = 0;
        } else {
          m_currentAnimationFinished = true;
          m_playing = false; // Optional: stop playing automatically
          break;             // Animation has finished, exit the loop
        }
      }
    }
  }

  // Get current frame UVs, etc.
  const std::pair<QVector2D, QVector2D> &currentFrameUVs() const {
    return m_animation.frameUVs[m_currentFrame];
  }

  void setAnimation(AnimationInfo animation, bool loop = false) {
    m_animation = animation;
    m_currentFrame = 0;
    m_timeAccumulatorMs = 0;
    m_playing = false;
    m_currentAnimationFinished = false;
    m_loop = loop;
  }

  void play() { m_playing = true; }
  void stop() { m_playing = false; }
  void reset() {
    m_currentFrame = 0;
    m_timeAccumulatorMs = 0;
  }

  int getCurrentFrame() const { return m_currentFrame; }
  bool isPlaying() const { return m_playing; }
  bool isFinished() const { return m_currentAnimationFinished; }

private:
  AnimationInfo m_animation;
  int m_currentFrame;
  int m_timeAccumulatorMs;
  bool m_playing;
  bool m_currentAnimationFinished;
  bool m_loop;
};

} // namespace Animations
} // namespace Graphics
