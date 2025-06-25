#include "Formation.h"
#include "Utils/Math/MathConstants.h"
#include <algorithm>

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

Formation &Formation::withSolidity(bool solidity) {
  m_solid = solidity;
  return *this;
}

Formation &Formation::withSpacing(const QPoint spacing) {
  m_spacing = spacing;
  return *this;
}

std::vector<QPoint> Formation::getPoints(QPoint referencePosition) const {
  switch (m_type) {
  case Type::RECTANGLE:
    return getRectPoints(referencePosition);
  case Type::TRIANGLE:
    return getTrianglePoints(referencePosition);
  case Type::CIRCLE:
    return getCirclePoints(referencePosition);
  }
}

std::vector<QPoint> Formation::getRectPoints(QPoint referencePosition) const {
  std::vector<QPoint> points;

  for (int row = 0; row < m_height; ++row) {
    for (int col = 0; col < m_width; ++col) {
      // For hollow rectangle, add points only on the perimeter
      if (!m_solid && row != 0 && row != m_height - 1 && col != 0 &&
          col != m_width - 1) {
        continue; // Skip internal points
      }
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
    int xOffset = row * m_spacing.x() / 2;

    for (int col = 0; col < numPointsInRow; ++col) {
      if (!m_solid && col != 0 && col != numPointsInRow - 1 && row != 0) {
        continue; // Skip internal points
      }
      int x = referencePosition.x() + xOffset + col * m_spacing.x();
      int y = referencePosition.y() + row * m_spacing.y();
      points.push_back(QPoint(x, y));
    }
  }

  return points;
}

std::vector<QPoint> Formation::getCirclePoints(QPoint referencePosition) const {
  std::vector<QPoint> points;
  int pointsInDiameter = m_width;
  int radiusInPoints = pointsInDiameter / 2;
  int spacing = m_spacing.x();

  int radius = radiusInPoints * spacing;
  QPoint center = QPoint(referencePosition.x(), referencePosition.y() + radius);

  if (m_solid) {
    for (int i = -radius; i <= radius; i += spacing) {
      for (int j = -radius; j <= radius; j += spacing) {
        if (i * i + j * j <= radius * radius) {
          points.push_back(QPoint(center.x() + i, center.y() + j));
        }
      }
    }
  } else {
    // Generate points along the circumference for a hollow circle
    int numPoints = std::max(2 * PI * radius / spacing,
                             static_cast<double>(pointsInDiameter));
    for (int i = 0; i < numPoints; ++i) {
      double angle = 2 * PI * i / numPoints;
      int x = center.x() + static_cast<int>(radius * std::cos(angle));
      int y = center.y() + static_cast<int>(radius * std::sin(angle));
      points.push_back(QPoint(x, y));
    }
  }

  return points;
}

} // namespace Levels
} // namespace Game
