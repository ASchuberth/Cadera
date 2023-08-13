#pragma once
#include "RenderUtil.hpp"

namespace CADERA_APP_NAMESPACE {

struct GridRotationAxis {
    glm::vec3 pos;
    glm::vec3 axis;
    float angle;
};


class Grid {

    public:


    Grid();
    std::vector<GridRotationAxis> createGridInstanceAxii();
    Vertex p1;
    Vertex p2;

    std::vector<Vertex> line;


};
}