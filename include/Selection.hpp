#pragma once
#include "Sketch.hpp"


namespace CADERA_APP_NAMESPACE {
namespace sel {
	float calcRatioToPlane(glm::vec3 currentRay, glm::vec3 cameraToPlaneDist, glm::vec3 planeOrigin,
		glm::vec3 planeNormal);

	glm::vec3 calcPOnPlane(glm::vec3 currentRay, glm::vec3 origin, glm::vec3 normal, glm::vec3 pos);

	glm::vec2 normalizeToVulkanCoords(glm::vec2 screenCoords, uint32_t surfaceWidth, uint32_t surfaceHeight);

	glm::vec4 clipToEyeSpace(glm::vec4 clipCoords, glm::mat4 projMat);

	glm::vec3 toWorldCoord(glm::vec4 eyeCoords, glm::mat4 viewMat);

	glm::vec3 calcCurrentRay(float x, float y, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width,
		uint32_t height);


	enum SelectionFlags {
		select_toggle,          // Used in callbacks to toggle selection
		select_isCTRL,                // Is CTRL pressed down
		select_single_point,
		select_double_point,
		select_multi_point,
		select_first_click,
		select_number_flags               // Number of flags for bitset
	};

	class Selector {

		sketch::Sketch *pActiveSketch = nullptr;

	public:

		std::bitset<select_number_flags> flags;

		glm::vec3 point;

		bool pointJustAdded;

		std::map<int, Point> selectedPoints;

		Selector();

		void setActiveSketch(sketch::Sketch* pSketch);

		void select(glm::vec3 mouseRay, glm::vec3 origin, glm::vec3 normal, glm::vec3 pos, glm::vec3 cross, bool isOrtho);

		static int selectPoint(glm::vec3 pointToAdd, std::map<int, Point> &points, float skScale);

        void update(std::map<int, Point> points);

        int add(glm::vec3 pointToAdd, std::map<int, Point> &points, float skScale);

		int remove(glm::vec3 pointToRemove, std::map<int, Point> &points, float skScale);

		bool existingPoint(glm::vec3 point);

		void setFlags();

		void clear();

		std::vector<glm::vec3> getVertices();

		std::vector<int> getSelectedPointIds();

	};
}
}