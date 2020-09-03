#pragma once
#include <glm/glm.hpp>
#include "Selection.hpp"

namespace CADERA_APP_NAMESPACE {

	const size_t NUM_CAM_FLAGS = 2;
	const int CAM_MOUSE_FIRST_PRESS = 0;
	const int CAM_PAN = 1;

	class Camera {

	public:

		Camera();

		std::bitset<NUM_CAM_FLAGS> flags;

		glm::vec3 pos;
		glm::vec3 focus;
		glm::vec3 cameraVec;

		glm::vec3 mouseRay;

		double xpos;
		double ypos;

		void zoom(float yoffset);

		void updateMouseRay(float x, float y, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width,
			                uint32_t height);

		void pan(glm::vec3 origin, glm::vec3 planeNormal);


	};

}

