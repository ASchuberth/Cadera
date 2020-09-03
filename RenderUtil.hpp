#pragma once
#include <glm/glm.hpp>



namespace CADERA_APP_NAMESPACE {

	const uint32_t BUF_SKETCH_LINES = 0;

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

		float step = -100.0f;
		for (int i = 0; i < 202; i++) {

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

