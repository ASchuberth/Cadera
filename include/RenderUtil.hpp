#pragma once




namespace CADERA_APP_NAMESPACE {

	const uint32_t BUF_SKETCH_GRID_LINE = 1;
	const uint32_t BUF_SKETCH_GRID_AXII = 2;
	const uint32_t BUF_SKETCH_POINTS = 3;
	const uint32_t BUF_SELECTION_POINTS = 4;
	const uint32_t BUF_TEXT_VERTICES = 5;
	const uint32_t BUF_TEXT_INDICES = 6;

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 col;
		
	};


}

