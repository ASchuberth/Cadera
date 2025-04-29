#pragma once
#include "ux/gui.hpp"

namespace CADERA_APP_NAMESPACE {

/// @brief Flags that can be toggled in a bitset to trigger events
enum CaderaFlags {

  /// Will trigger deletion of the currently selected object
  cadera_delete,
  /// Number of flags to be used in the creation of the bitset
  cadera_num_flags
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

  std::map<int, Model> Models;

  int modelIdCounter;

	public:
		
		CADRender Render;
		sketch::Sketch Sketch;

  std::bitset<cadera_num_flags> flags;

  Cadera();

  ~Cadera();

  void SketchEvents();

  void updateSelectionPoints(std::vector<int> ids);

        void run();

		void mainLoop();
	};

} // namespace CADERA_APP_NAMESPACE