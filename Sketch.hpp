#pragma once
#include "Point.hpp"
#include "Model.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sketch {
	
	enum SketchToolFlags {
		skt_tool_active,       // Used to toggle adding of point to sketch in the callbacks
		skt_point_tool,        // Activate the point tool
		skt_num_flags          // Number of flags for sketch flags bitset
	};

	class Sketch : public Model {

		int featureCounter;

		

	public:
		
		float* mCamDistance;

		std::map<int, Point> Points;
		std::map<int, Relation> mRelations;

		Sketch();
		Sketch(int i);
		
		std::bitset<skt_num_flags> flags;

		int getFeatureCounter();

		void setCameraDistance(float* camDistance);

		void activatePointTool();

		void deactivateTools();

		void add(glm::vec3 point);

		void addRelation(const std::vector<int> &ids, RelationType Type);

		void deleteRelation(int id);

		void clearRelations();

		size_t numRelations();

		Point* addPoint(glm::vec3 point);

		void deletion(std::vector<int> ids);
		
		std::vector<glm::vec3> getVertices();
		


	};

}
}
