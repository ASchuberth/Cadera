#include "pch.hpp"
#include "grid.hpp"


namespace CADERA_APP_NAMESPACE {

Grid::Grid() {

    size = 10;
    spacing = 1.0f;

    p1 = { {0.0f, -size, 0.0f}, {.0f, .0f, .0f} };
	p2 = { {0.0f,  size, 0.0f}, {.0f, .0f, .0f} };

    line = { p1, p2 };
}

std::vector<GridRotationAxis> Grid::createGridInstanceAxii() {

		std::vector<GridRotationAxis> axii;


        p1 = { {0.0f, -size * spacing, 0.0f}, {.0f, .0f, .0f} };
        p2 = { {0.0f,  size * spacing, 0.0f}, {.0f, .0f, .0f} };

        line = { p1, p2 };

		float position = -size * spacing;
		for (int i = 0; i < 2*size+1 ; i++) {

			GridRotationAxis axis = {
				{0.0f, 0.0f, position},
				{1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
				0.0f
			};


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

}
