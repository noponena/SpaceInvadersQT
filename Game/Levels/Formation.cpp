#include "Formation.h"

namespace Game {
namespace Levels {

Formation::Formation() {}

Formation &Formation::withType(Type type) {
  m_type = type;
  return *this;
}

Formation &Formation::withSize(int width, int height) {
  m_width = width;
  m_height = height;
  return *this;
}

Formation &Formation::withSpacing(const QPoint spacing) {
  m_spacing = spacing;
  return *this;
}

std::vector<QPoint> Formation::getPoints(QPoint referencePosition) const {
  return getRectPoints(referencePosition);
}

std::vector<QPoint> Formation::getRectPoints(QPoint referencePosition) const {
  std::vector<QPoint> points;

  for (int row = 0; row < m_height; ++row) {
    for (int col = 0; col < m_width; ++col) {
      int x = referencePosition.x() + col * m_spacing.x();
      int y = referencePosition.y() + row * m_spacing.y();
      points.push_back(QPoint(x, y));
    }
  }

  return points;
}

std::vector<QPoint>
Formation::getTrianglePoints(QPoint referencePosition) const {
  std::vector<QPoint> points;

  for (int row = 0; row < m_height; ++row) {
    int numPointsInRow = m_width - row;
    int xOffset = row * m_spacing.x() / 2; // Centering each row

    for (int col = 0; col < numPointsInRow; ++col) {
      int x = referencePosition.x() + xOffset + col * m_spacing.x();
      int y = referencePosition.y() + row * m_spacing.y();
      points.push_back(QPoint(x, y));
    }
  }

  return points;
}

std::vector<QPoint> Formation::getCirclePoints(QPoint referencePosition) const {
  std::vector<QPoint> points;
  int radius = m_width; // Assuming m_width is the radius in points
  int diameter = radius * 2;

  for (int i = 0; i < diameter; ++i) {
    for (int j = 0; j < diameter; ++j) {
      int dx = radius - i;
      int dy = radius - j;
      if (dx * dx + dy * dy <= radius * radius) {
        int x = referencePosition.x() + i * m_spacing.x();
        int y = referencePosition.y() + j * m_spacing.y();
        points.push_back(QPoint(x, y));
      }
    }
  }

  return points;
}

} // namespace Levels
} // namespace Game
