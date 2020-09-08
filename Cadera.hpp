#pragma once
#include "gui.hpp"
#include "callbacks.hpp"

namespace CADERA_APP_NAMESPACE {

	enum CaderaFlags {
		cadera_delete,
		cadera_num_flags
	};

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

		std::bitset<cadera_num_flags> flags;

		Cadera();

		~Cadera();

		void run();

		void mainLoop();
	};

}