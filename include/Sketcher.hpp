#pragma once
#include "Point.hpp"
#include "Model.hpp"



namespace CADERA_APP_NAMESPACE {
namespace sketch {
	
	enum SketchToolFlags {
		skt_tool_active,       // Used to toggle adding of point to sketch in the callbacks
		skt_point_tool,        // Activate the point tool
		skt_note_tool,         // Activate the note tool
		skt_num_flags          // Number of flags for sketch flags bitset
	};

	class Sketcher {

	};

}
}

