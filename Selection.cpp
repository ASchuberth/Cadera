#include "pch.hpp"
#include "Selection.hpp"

namespace CADERA_APP_NAMESPACE {
namespace sel {
	float calcRatioToPlane(glm::vec3 currentRay, glm::vec3 cameraToPlaneDist, glm::vec3 planeOrigin,
		glm::vec3 planeNormal) {

		float ratio;

		glm::vec3 rayProjToPlaneNormalDist;
		glm::vec3 cameraToNormalPlaneDist;

		cameraToNormalPlaneDist = (glm::dot(cameraToPlaneDist, planeNormal) * planeNormal);
		rayProjToPlaneNormalDist = (glm::dot(currentRay, planeNormal) * planeNormal);

		ratio = std::fabs(glm::length(cameraToNormalPlaneDist) / glm::length(rayProjToPlaneNormalDist));

		return ratio;

	}

	glm::vec3 calcPOnPlane(glm::vec3 currentRay, glm::vec3 origin, glm::vec3 normal, glm::vec3 pos) {

		glm::vec3 cameraToPlaneDist;
		float ratio;
		glm::vec3 vecCamToPOnPlane;
		glm::vec3 POnPlaneCoord;


		cameraToPlaneDist = { pos.x - origin.x, pos.y - origin.y, pos.z - origin.z };

		ratio = calcRatioToPlane(currentRay, cameraToPlaneDist, origin, normal);


		vecCamToPOnPlane = ratio * currentRay;
		POnPlaneCoord = cameraToPlaneDist + vecCamToPOnPlane + origin;

		return POnPlaneCoord;

	}

	glm::vec2 normalizeToVulkanCoords(glm::vec2 screenCoords, uint32_t surfaceWidth, uint32_t surfaceHeight) {
		glm::vec2 normCoords;

		normCoords.x = 2.0f * screenCoords.x / static_cast<float>(surfaceWidth) - 1.0;
		normCoords.y = 2.0f * screenCoords.y / static_cast<float>(surfaceHeight) - 1.0;

		return normCoords;
	}

	glm::vec4 clipToEyeSpace(glm::vec4 clipCoords, glm::mat4 projMat) {
		glm::vec4 eyeCoords;
		glm::mat4 inverseProjection;

		inverseProjection = glm::inverse(projMat);
		eyeCoords = inverseProjection * clipCoords;
		eyeCoords.z = -1.0f;
		eyeCoords.w = 0.0f;


		return eyeCoords;
	}

	glm::vec3 toWorldCoord(glm::vec4 eyeCoords, glm::mat4 viewMat) {
		glm::vec3 mouseRay;
		glm::vec4 rayWorld;
		glm::mat4 inverseView;

		inverseView = glm::inverse(viewMat);
		rayWorld = inverseView * eyeCoords;
		mouseRay = { rayWorld.x, rayWorld.y, rayWorld.z };
		mouseRay = glm::normalize(mouseRay);


		return mouseRay;
	}

	glm::vec3 calcCurrentRay(float x, float y, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width, uint32_t height) {
		glm::vec2 screenCoords;
		glm::vec2 normCoords;
		glm::vec4 clipCoords;
		glm::vec4 eyeCoords;

		glm::vec3 currentRay;

		screenCoords = { x, y };

		normCoords = normalizeToVulkanCoords(screenCoords, width, height);
		clipCoords = { normCoords.x, normCoords.y, -1.0f, 1.0f };
		eyeCoords = clipToEyeSpace(clipCoords, projMat);

		currentRay = toWorldCoord(eyeCoords, viewMat);

		return currentRay;
	}

	Selector::Selector() {

		point = { 0.0f, 0.0f, 0.0f };

	}

	void Selector::select(glm::vec3 mouseRay, glm::vec3 origin, glm::vec3 normal, glm::vec3 pos, bool isOrtho) {

		if (isOrtho) {
			point.x = 0.0f;
			point.y = mouseRay.x + pos.y;
			point.z = mouseRay.y + pos.z;
		}
		else {
			point = calcPOnPlane(mouseRay, origin, normal, pos);
		}

	}

	int Selector::selectPoint(glm::vec3 pointToAdd, std::map<int, Point> &points, float skScale) {

		std::vector<int> sortedIndices;
		double sensitivity = skScale / 800.0f;

		for (const auto& point : points) {
			glm::vec3 pointVec;
			double length;

			pointVec = pointToAdd - point.second.pos;


			length = glm::length2(pointVec);

			if (length < sensitivity) {
				return point.first;
			}
		}

		return -1;
	}

	int Selector::add(glm::vec3 pointToAdd, std::map<int, Point>& points, float skScale) {

		// Clear selected points if CTRL isn't pressed
		if (!flags.test(select_isCTRL)) {
			selectedPoints.clear();
			setFlags();
		}

		int selectedPointId = selectPoint(pointToAdd, points, skScale);

		// If a point is selected
		if (selectedPointId >= 0) {

			// If not in selectedPoints, add it, else remove it
			if (selectedPoints.find(selectedPointId) == selectedPoints.end()) {
				
				selectedPoints[selectedPointId] = points[selectedPointId];
				setFlags();
				
				return selectedPointId;

			}
			else {

				selectedPoints.erase(selectedPointId);
				setFlags();

				return selectedPointId;

			}
		}

		// If not CTRL and selectedPointId < 0
		if (!flags.test(select_isCTRL)) {

			selectedPoints.clear();
			setFlags();

		}

		return selectedPointId;

	}

	void Selector::setFlags() {

		if (selectedPoints.size() == 1) {
			
			flags.set(select_single_point);
			
			flags.reset(select_double_point);
			flags.reset(select_multi_point);

		}
		else if (selectedPoints.size() == 2) {
			
			flags.set(select_double_point);
			
			flags.reset(select_single_point);
			flags.reset(select_multi_point);

		}
		else if (selectedPoints.size() >= 3) {
			
			flags.set(select_multi_point);

			flags.reset(select_single_point);
			flags.reset(select_double_point);

		}
		else {
			
			flags.reset(select_single_point);
			flags.reset(select_double_point);
			flags.reset(select_multi_point);

		}
		

	}

	void Selector::clear() {
		selectedPoints.clear();
	}

	std::vector<glm::vec3> Selector::getVertices() {

		std::vector<glm::vec3> vertices;

		for (const auto& p : selectedPoints) {
			vertices.push_back(p.second.pos);
		}

		return vertices;
	}

	std::vector<int> Selector::getSelectedPointIds() {

		std::vector<int> ids;

		for (auto& P : selectedPoints) {
			ids.push_back(P.second.getId());
		}

		return ids;
	}
	
}
}