#pragma once

#include "Sketch.hpp"


namespace CADERA_APP_NAMESPACE {

	const uint32_t BUF_SKETCH_POINTS = 3;
	const uint32_t BUF_SELECTION_POINTS = 4;
	const uint32_t BUF_TEXT_VERTICES = 5;
	const uint32_t BUF_TEXT_INDICES = 6;

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 col;
		
	};

	struct GridRotationAxis {
		glm::vec3 pos;
		glm::vec3 axis;
		float angle;
	};

	inline std::vector<GridRotationAxis> createGridInstanceAxii() {

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

