#pragma once
#include "gui.hpp"
#include "callbacks.hpp"

namespace CADERA_APP_NAMESPACE {

	class Cadera {

	private:
		

		std::vector<cad::Vertex> ptVertices = {
			{{ 0.0f,   0.0f,  0.0f},  {0.0f, 1.0f, 0.0f}},
			{{ 0.00f,  3.0f,  0.00f}, {0.0f, 1.0f, 0.0f}},
			{{ 0.00f,  0.0f,  0.00f}, {1.0f, 0.0f, 0.0f}},
			{{ 0.00f,  0.0f,  3.00f}, {1.0f, 0.0f, 0.0f}}
		};

		void initCallbacks();

		void switchCallbacks();
		
		std::map<int, Model> Models;

		int modelIdCounter;

	public:
		
		CADRender Render;
		sketch::Sketch Sketch;

		Cadera();

		~Cadera();

		void run();

		void mainLoop();
	};

}