#pragma once
#include "Feature.hpp"

namespace CADERA_APP_NAMESPACE {

class Point : public Feature {

public:
  glm::vec3 pos;
  std::list<int> relationIds;
  int noteId;

  Point();
  Point(int id, glm::vec3 pos);

  inline void removeRelationId(int id) {

    std::list<int>::iterator iter = relationIds.begin();

    while (iter != relationIds.end()) {

      if (*iter == id) {
        iter = relationIds.erase(iter);
      } else {
        ++iter;
      }
    }
  }
};
} // namespace CADERA_APP_NAMESPACE