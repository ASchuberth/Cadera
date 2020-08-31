#pragma once
#include "CADRender.hpp"

namespace CADERA_APP_NAMESPACE {

	class Cadera {

	private:
		CADRender Render;

		std::vector<cad::Vertex> ptVertices = {
			{{ 0.0f,   0.0f,  1.0f},  {1.0f, 1.0f, 1.0f}},
			{{ 0.00f,  1.0f,  0.00f}, {1.0f, 1.0f, 0.0f}},
			{{ 0.00f,  0.0f,  0.00f}, {1.0f, 0.0f, 1.0f}},
			{{ 0.00f,  1.0f,  1.00f}, {1.0f, 0.0f, 1.0f}}
		};

	public:

		Cadera();

		~Cadera();

		void run();

		void mainLoop();
	};

}