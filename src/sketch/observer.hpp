#pragma once
#include "textrender.hpp"
#include "grid.hpp"

namespace CADERA_APP_NAMESPACE {

class RenderItems {

  friend std::ostream &operator<<(std::ostream &os, const RenderItems &ri) {

    os << "RenderItems:" << std::endl;

    if (ri.points.empty())
      return os;
    os << "Points:" << std::endl;
    for (const auto &p : ri.points) {

      os << std::format("P,{:.2f},{:.2f},{:.2f}", p.x, p.y, p.z);
      os << std::endl;
    }
    os << std::endl;
    return os;
  }

public:
  std::vector<glm::vec3> points;
  std::vector<GridRotationAxis> gridAxii;
  std::vector<glm::vec3> gridLine;
  // std::vector<glm::vec3> Points;
  // std::vector<glm::vec3> Points;
};

class Observer {

public:
  Observer();
  virtual ~Observer();

  virtual void onNotify(int id, const RenderItems &renderables);

private:
};
}