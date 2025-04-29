#include "grid.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

Grid::Grid() {

  size = 10;
  spacing = 1.0f;

  p1 = {0.0f, -size, 0.0f};
  p2 = {0.0f, size, 0.0f};

  line = {p1, p2};

  gUp = {0.0f, 1.0f, 0.0f};
  gCross = {1.0f, 0.0f, 0.0f};
  gNormal = {0.0f, 0.0f, 1.0f};
}

void Grid::setGridOrientation(glm::vec3 normal, glm::vec3 up, glm::vec3 cross) {
  gNormal = normal;
  gUp = up;
  gCross = cross;
}

std::vector<GridRotationAxis> Grid::createGridInstanceAxii() {

  std::vector<GridRotationAxis> axii;

  p1 = (-size * spacing) * gUp;
  p2 = (size * spacing) * gUp;

  line = {p1, p2};

  float position = -size * spacing;
  for (int i = 0; i < 2 * size + 1; i++) {

    GridRotationAxis axis = {
        position * gCross, gNormal, {0.0f, 0.0f, 0.0f}, 0.0f};

    if (i == size) {
      axis.color = {0.0f, 1.0f, 0.0f};
    }

    axii.push_back(axis);

    axis.angle = 90.0f;

    if (i == size) {
      axis.color = {1.0f, 0.0f, 0.0f};
    }

    axii.push_back(axis);

    position += spacing;
  }

  return axii;
}

} // namespace CADERA_APP_NAMESPACE
