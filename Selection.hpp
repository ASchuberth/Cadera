#pragma once
#include "Point.hpp"
#include <glm/gtx/norm.hpp>

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
		numFlags               // Number of flags for bitset
	};

	class Selector {

	public:

		std::bitset<numFlags> flags;

		glm::vec3 point;

		std::map<int, Point> selectedPoints;

		Selector();

		void select(glm::vec3 mouseRay, glm::vec3 origin, glm::vec3 normal, glm::vec3 pos, bool isOrtho);

		static int selectPoint(glm::vec3 pointToAdd, std::map<int, Point> &points, float skScale);

		int add(glm::vec3 pointToAdd, std::map<int, Point> &points, float skScale);

		void setFlags();

		void clear();

		std::vector<glm::vec3> getVertices();

		std::vector<int> getSelectedPointIds();

	};
}
}