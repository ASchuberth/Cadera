#include "pch.hpp"
#include "grid.hpp"


namespace CADERA_APP_NAMESPACE {

Grid::Grid() {

    p1 = { {0.0f, -1000.0f, 0.0f}, {.0f, .0f, .0f} };
	p2 = { {0.0f,  1000.0f, 0.0f}, {.0f, .0f, .0f} };

    line = { p1, p2 };
}

std::vector<GridRotationAxis> Grid::createGridInstanceAxii() {

		std::vector<GridRotationAxis> axii;

		float step = -1000.0f;
		for (int i = 0; i < 2002; i++) {

			GridRotationAxis axis = {
				{0.0f, 0.0f, step},
				{1.0f, 0.0f, 0.0f},
				0.0f
			};


			axii.push_back(axis);

			axis.angle = 90.0f;

			axii.push_back(axis);

			step += 1.0f;
		}


		return axii;
	}

}
