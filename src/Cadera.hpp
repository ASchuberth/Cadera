#pragma once
#include "ux/gui.hpp"





namespace CADERA_APP_NAMESPACE {

	/// @brief Flags that can be toggled in a bitset to trigger events
	enum CaderaFlags {
		
		/// Triggers adding sketch to Cadera
		acadera_add_sketch,
		/// Will trigger deletion of the currently selected object
		acadera_delete,
		/// Number of flags to be used in the creation of the bitset 
		acadera_num_flags  
	};

/**
 * @brief Cadera
 * @author Austin Schuberth
 * @page Cadera
 * 
 */
	class Cadera {

	private:
		
		void initCallbacks();
		
		std::map<int, std::unique_ptr<Model>> Models;
		
		int modelIdCounter;

	public:
		
		CADRender Render;
		sketch::Sketch Sketch;

		Events CaderaEvents;

		std::bitset<cadera_num_flags> flags;

		Cadera();

		~Cadera();

		void addModel(Model M);

        void SketchEvents();

        void updateSelectionPoints(std::vector<int> ids);

        void run();

		void mainLoop();
	};

}