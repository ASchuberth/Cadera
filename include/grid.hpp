#pragma once
#include "RenderUtil.hpp"

namespace CADERA_APP_NAMESPACE {

struct GridRotationAxis {
    glm::vec3 pos;
    glm::vec3 axis;
    glm::vec3 color;
    float angle;
};


class Grid {

    public:


    Grid();
    
    /// @brief To generate the values needed to instance each grid line from the base line
    /// @return Vector to be used for rendering the instances
    std::vector<GridRotationAxis> createGridInstanceAxii();
    
    Vertex p1;
    Vertex p2;

    /// @brief The container for rendering the base line
    std::vector<Vertex> line;

    /// @brief The size of the grid. The number of times the grid gets incremented.
    int32_t size;

    /// @brief The spacing between each line in the grid
    float spacing;


};
}