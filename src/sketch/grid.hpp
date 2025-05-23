#pragma once

namespace CADERA_APP_NAMESPACE {

/// @brief The axis of rotation for the grid lines
struct GridRotationAxis {
  glm::vec3 pos;
  glm::vec3 axis;
  glm::vec3 color;
  float angle;
};

class Grid {

public:
  Grid();

  void setGridOrientation(glm::vec3 normal, glm::vec3 up, glm::vec3 cross);

  /// @brief To generate the values needed to instance each grid line from the
  /// base line
  /// @return Vector to be used for rendering the instances
  std::vector<GridRotationAxis> createGridInstanceAxii();

  glm::vec3 p1;
  glm::vec3 p2;

  glm::vec3 gNormal;
  glm::vec3 gUp;
  glm::vec3 gCross;

  /// @brief The container for rendering the base line
  std::vector<glm::vec3> line;

  /// @brief The size of the grid. The number of times the grid gets
  /// incremented.
  int32_t size;

  /// @brief The spacing between each line in the grid
  float spacing;
};
} // namespace CADERA_APP_NAMESPACE