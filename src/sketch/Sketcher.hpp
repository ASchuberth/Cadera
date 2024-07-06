#pragma once
#include "Point.hpp"
#include "Model.hpp"



namespace CADERA_APP_NAMESPACE {
namespace sketch {
	
	enum SketchToolFlags {
		skt_active,            // Cadera is in sketch mode
		skt_move_points,       // Triggers the moving of points
		skt_points_have_moved,  // Stops deselection when points have moved
		skt_tool_active,       // Used to toggle adding of point to sketch in the callbacks
		skt_point_tool,        // Activate the point tool
		skt_note_tool,         // Activate the note tool
		skt_event_tool_deactivated,  // Stops the rendering of tool indicators
		skt_num_flags          // Number of flags for sketch flags bitset
	};

	class Sketcher {

		Sketcher();

		~Sketcher();

		
		

		std::bitset<skt_num_flags> flags;

		void add(glm::vec3 pointToAdd);



	};

}
}

