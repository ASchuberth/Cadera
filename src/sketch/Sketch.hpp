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


	// Returns a list with any elements in v2 that aren't in v1
	template<typename T>
	inline std::list<T> getDifferenceVector(std::list<T> v1, std::list<T> v2) {

		std::list<T> differenceVector;

		if (v1.empty())
			return v2;

		for (const auto& elem : v2) {

			bool isFound = std::find(v1.begin(), v1.end(), elem) != v1.end();

			if (!isFound) {

				differenceVector.push_back(elem);

			}

		}

		return differenceVector;

	}

	class Sketch : public Model {

		int featureCounter;

	public:
		
		float* mCamDistance;

		Grid mGrid;

		std::map<int, Point> Points;
		std::map<int, Relation> mRelations;

		std::string noteText;

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

		void movePoints(std::map<int, Point> ids, const glm::vec3 &POnPlane, const bool& isFirstClick);

		void deletion(std::vector<int> ids);

		std::vector<Vertex> getGridLine();

		std::vector<GridRotationAxis> getGridAxii();
		
		std::vector<glm::vec3> getVertices(std::vector<glm::vec3>& colors);

		std::vector<txt::Text> getRelationTexts();
		


	};

}
}
