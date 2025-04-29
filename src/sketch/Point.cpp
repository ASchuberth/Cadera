#include "Point.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

Point::Point() { noteId = -1; }

Point::Point(int id, glm::vec3 pos) {

  setId(id);
  this->pos = pos;
}
} // namespace CADERA_APP_NAMESPACE