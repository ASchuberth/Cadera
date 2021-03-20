#pragma once
#include "Point.hpp"
#include "Model.hpp"
#include "TextRender.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {
	
	enum SketchToolFlags {
		skt_tool_active,       // Used to toggle adding of point to sketch in the callbacks
		skt_point_tool,        // Activate the point tool
		skt_note_tool,         // Activate the note tool
		skt_num_flags          // Number of flags for sketch flags bitset
	};

	class Sketch : public Model {

		int featureCounter;

		

	public:
		
		float* mCamDistance;

		std::map<int, Point> Points;
		std::map<int, Relation> mRelations;

		char text[1024 * 16];


		Sketch();
		Sketch(int i);
		
		std::bitset<skt_num_flags> flags;

		int getFeatureCounter();

		void setCameraDistance(float* camDistance);

		void activatePointTool();

		void activateNoteTool();

		void deactivateTools();

		void add(glm::vec3 point);

		void addOrigin();

		void addRelation(const std::vector<int> &ids, RelationType Type);

		void deleteRelation(int id);

		void clearRelations();

		size_t numRelations();

		Point* addPoint(glm::vec3 point);

		Point* addConstructionPoint(glm::vec3 point);

		Point* addNotePoint(glm::vec3 point);

		void deletion(std::vector<int> ids);
		
		std::vector<glm::vec3> getVertices(std::vector<glm::vec3>& colors);
		


	};

}
}
