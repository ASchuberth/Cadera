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

		void loadFonts();

		void run();

		void mainLoop();
	};

}