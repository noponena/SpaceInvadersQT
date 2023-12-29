#ifndef GAME_LEVELS_FORMATION_H
#define GAME_LEVELS_FORMATION_H

#include <QPoint>
#include <vector>

namespace Game {
namespace Levels {

class Formation {
public:
  enum class Type {
    RECTAGLE,
    TRIANGLE,
    CIRCLE,
  };

  Formation();
  Formation &withType(Type type);
  Formation &withSize(int width, int height);
  Formation &withSpacing(const QPoint spacing);

  std::vector<QPoint> getPoints(QPoint referencePosition) const;

private:
  Type m_type;
  int m_width;
  int m_height;
  QPoint m_spacing;

  std::vector<QPoint> getRectPoints(QPoint referencePosition) const;
  std::vector<QPoint> getTrianglePoints(QPoint referencePosition) const;
  std::vector<QPoint> getCirclePoints(QPoint referencePosition) const;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_FORMATION_H
