#pragma once

namespace CADERA_APP_NAMESPACE {

enum class BufferName {

  sketch_grid_line  = 0,
  sketch_grid_axii  = 1,
  sketch_points     = 2,
  selection_points  = 3,
  text_vertices     = 4,
  text_indices      = 5,
  sketch_point_tool = 6,
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 col;
};

} // namespace CADERA_APP_NAMESPACE
