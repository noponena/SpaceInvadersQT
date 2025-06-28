#ifndef GAME_LEVELS_FORMATION_H
#define GAME_LEVELS_FORMATION_H

#include <QVector2D>
#include <vector>

namespace Game {
namespace Levels {

class Formation {
public:
  enum class Type {
    RECTANGLE,
    TRIANGLE,
    CIRCLE,
  };

  Formation();
  Formation &withType(Type type);
  Formation &withSize(int width, int height);
  Formation &withSolidity(bool solidity);
  Formation &withSpacing(const QVector2D spacing);

  std::vector<QVector2D> getPoints(QVector2D referencePosition) const;

private:
  Type m_type;
  int m_width;
  int m_height;
  bool m_solid;
  QVector2D m_spacing;

  std::vector<QVector2D> getRectPoints(QVector2D referencePosition) const;
  std::vector<QVector2D> getTrianglePoints(QVector2D referencePosition) const;
  std::vector<QVector2D> getCirclePoints(QVector2D referencePosition) const;
};

} // namespace Levels
} // namespace Game

#endif // GAME_LEVELS_FORMATION_H
