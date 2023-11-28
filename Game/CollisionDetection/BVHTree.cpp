#include "BVHTree.h"
#include <algorithm>

namespace Game {
namespace CollisionDetection {

BVHTree::BVHTree() : m_splitX(true) {}

BVHTree::~BVHTree() {}

std::shared_ptr<BVHNode>
BVHTree::build(std::vector<GameObjects::GameObject *> &objects) {
  if (objects.empty()) {
    return nullptr;
  }

  m_processedPairs.clear();

  m_intersectionCounter = 0;

  // Decide on the splitting axis before splitting
  m_splitX = !m_splitX; // shouldSplitX(objects);

  std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();

  if (objects.size() == 1) {
    // Handle leaf node with a single object
    node->objects = objects;
    node->bbox = calculateBoundingBox(objects);
    node->leftChild = nullptr;
    node->rightChild = nullptr;
  } else {
    // Split objects into two groups
    auto [leftObjects, rightObjects] = splitObjects(objects, m_splitX);

    // Recursively build child nodes
    node->leftChild = build(leftObjects);
    node->rightChild = build(rightObjects);

    // Calculate bounding box for this node
    node->bbox =
        combineBoundingBoxes(node->leftChild->bbox, node->rightChild->bbox);
  }

  return node;
}

std::vector<GameObjects::GameObject *>
BVHTree::query(std::shared_ptr<BVHNode> node,
               GameObjects::GameObject *queryObject) {
  QRectF queryBbox = queryObject->getBoundingBox();
  if (!node || !node->bbox.intersects(queryBbox))
    return {};

  // If this is an internal node, check collisions between left and right
  // children
  if (node->leftChild && node->rightChild) {
    std::vector<GameObjects::GameObject *> leftResults;
    std::vector<GameObjects::GameObject *> rightResults;

    if (node->leftChild->bbox.intersects(queryBbox)) {
      leftResults = query(node->leftChild, queryObject);
    }
    if (node->rightChild->bbox.intersects(queryBbox)) {
      rightResults = query(node->rightChild, queryObject);
    }

    // Combine the results from left and right children
    leftResults.insert(leftResults.end(), rightResults.begin(),
                       rightResults.end());
    return leftResults;
  } else {
    // Leaf node processing
    std::vector<GameObjects::GameObject *> result;
    for (size_t i = 0; i < node->objects.size(); i++) {

      auto sortedPair =
          (queryObject->id() < node->objects[i]->id())
              ? std::make_pair(queryObject->id(), node->objects[i]->id())
              : std::make_pair(node->objects[i]->id(), queryObject->id());

      if (m_processedPairs.find(sortedPair) == m_processedPairs.end()) {

        if (queryObject->isCollidable() && node->objects[i]->isCollidable()) {
          if (canCollide(queryObject->objectTypes(),
                         node->objects[i]->objectTypes())) {
            if (node->objects[i]->getBoundingBox().intersects(
                    queryObject->getBoundingBox())) {
              result.push_back(node->objects[i]);
            }
          }
        }
        m_processedPairs.insert(sortedPair);
      }
    }
    return result;
  }
}

void BVHTree::clearProcessedPairs() { m_processedPairs.clear(); }

QRectF BVHTree::calculateBoundingBox(
    const std::vector<GameObjects::GameObject *> &objects) {
  QRectF bbox;
  for (const auto &obj : objects) {
    bbox = bbox.united(obj->getBoundingBox());
  }
  return bbox;
}

std::pair<std::vector<GameObjects::GameObject *>,
          std::vector<GameObjects::GameObject *>>
BVHTree::splitObjects(std::vector<GameObjects::GameObject *> &objects,
                      bool splitX) {
  auto medianIt = objects.begin() + objects.size() / 2;
  std::nth_element(
      objects.begin(), medianIt, objects.end(),
      [splitX](const GameObjects::GameObject *a,
               const GameObjects::GameObject *b) {
        return splitX ? a->getCenterPosition().x() < b->getCenterPosition().x()
                      : a->getCenterPosition().y() < b->getCenterPosition().y();
      });

  std::vector<GameObjects::GameObject *> leftObjects(objects.begin(), medianIt);
  std::vector<GameObjects::GameObject *> rightObjects(medianIt, objects.end());

  return {leftObjects, rightObjects};
}

QRectF BVHTree::combineBoundingBoxes(const QRectF &bbox1, const QRectF &bbox2) {

  return bbox1.united(bbox2);
}

bool BVHTree::shouldSplitX(
    const std::vector<GameObjects::GameObject *> &objects) {
  if (objects.empty())
    return true;

  // Initialize min and max values for both axes
  qreal minX = objects[0]->getBoundingBox().left();
  qreal maxX = objects[0]->getBoundingBox().right();
  qreal minY = objects[0]->getBoundingBox().top();
  qreal maxY = objects[0]->getBoundingBox().bottom();

  // Iterate over all objects to find the range
  for (const auto &obj : objects) {
    QRectF bbox = obj->getBoundingBox();
    minX = std::min(minX, bbox.left());
    maxX = std::max(maxX, bbox.right());
    minY = std::min(minY, bbox.top());
    maxY = std::max(maxY, bbox.bottom());
  }

  // Calculate range for both axes
  qreal rangeX = maxX - minX;
  qreal rangeY = maxY - minY;

  // Return true if range in X is greater than or equal to range in Y
  return rangeX >= rangeY;
}

} // namespace CollisionDetection
} // namespace Game
