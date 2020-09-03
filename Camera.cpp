#include "pch.hpp"
#include "Camera.hpp"

namespace CADERA_APP_NAMESPACE {




	Camera::Camera()
	{
		 pos = { 20.0f, 0.0f, 0.0f };
		 focus = { 0.0f, 0.0f, 0.0f };
		 cameraVec = { 1.0f, 0.0f, 0.0f };
		 
		 mouseRay = { 0.0f, 0.0f, 0.0f };
	}

	void Camera::zoom(float yoffset) {

		if (yoffset > 0) {
			pos -= 1.0f * glm::normalize(cameraVec);
		}
		else if (yoffset < 0) {
			pos += 1.0f * glm::normalize(cameraVec);
		}

	}

	void Camera::updateMouseRay(float x, float y, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width, 
		                        uint32_t height) {

		mouseRay = calcCurrentRay(x, y, viewMat, projMat, width, height);

	}

	void Camera::pan(glm::vec3 origin, glm::vec3 planeNormal) {

	
		static glm::vec3 prevMouseRay = mouseRay;
		glm::vec3 p1, p2, diff;

		if (flags.test(CAM_MOUSE_FIRST_PRESS)) {
			
			prevMouseRay = mouseRay;
			flags.reset(CAM_MOUSE_FIRST_PRESS);

		}

		p1 = calcPOnPlane(prevMouseRay, origin, planeNormal, pos);
		p2 = calcPOnPlane(mouseRay, origin, planeNormal, pos);

		diff = p2 - p1;

		
		pos -= diff;
		focus -= diff;
		

		prevMouseRay = mouseRay;

	}

}


