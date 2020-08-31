#pragma once
#include <glm/glm.hpp>



namespace CADERA_APP_NAMESPACE {

	const uint32_t BUF_SKETCH_LINES = 0;

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 col;
		
	};

	inline std::vector<Vertex> createGridPoints() {

		std::vector<Vertex> points;

		return points;
	}




}

