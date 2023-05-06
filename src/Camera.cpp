#include "pch.hpp"
#include "Camera.hpp"

namespace CADERA_APP_NAMESPACE {

namespace cam {


	Camera::Camera()
	{
		pos = { -20.0f, 0.0f, 0.0f };
		focus = { 0.0f, 0.0f, 0.0f };
		cameraVec = { 1.0f, 0.0f, 0.0f };
		xpos = 0.0;
		ypos = 0.0;
		mouseRay = { 0.0f, 0.0f, 0.0f };
		left = -10.0f;
		camDistance = 0.0f;

	}

	void Camera::update() {

		camDistance = glm::length(pos - focus);

	}

	void Camera::zoom(float yoffset) {

		if (flags.test(ortho)) {
			
			if (yoffset > 0 && left < -1.02) {
				left += 1.0f;
			}
			else if (yoffset < 0) {
				left -= 1.0f;
			}
		}
		else {

			glm::vec3 normalPos = {0.0f, pos.y, pos.z};

			if (yoffset > 0) {
				pos -= 1.0f * glm::normalize(cameraVec);
			}
			else if (yoffset < 0 && glm::length2(pos - focus) > 1.02f) {
				pos += 1.0f * glm::normalize(cameraVec);
			}
		}

	}

	void Camera::updateMouseRay(float x, float y, glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width,
		                        uint32_t height) {

		if (flags.test(ortho)) {
			mouseRay = glm::unProject(glm::vec3(x, y, 0.0f), modelMat, projMat,
				                      glm::vec4(0.0f, 0.0f, width, height));
		}
		else {
			mouseRay = sel::calcCurrentRay(x, y, viewMat, projMat, width, height);
		}

	}

	/**
	 * @brief 
	 * 
	 * @param origin 
	 * @param planeNormal 
	 */
	void Camera::pan(glm::vec3 origin, glm::vec3 planeNormal) {


		static glm::vec3 prevMouseRay = mouseRay;
		glm::vec3 p1, p2, diff;

		if (flags.test(mouseFirstPressed)) {

			prevMouseRay = mouseRay;
			flags.reset(mouseFirstPressed);

		}

		if (flags.test(ortho)) {

			diff = mouseRay - prevMouseRay;

			pos.y -= diff.y;
			pos.z -= diff.x;
			focus.y -= diff.y;
			focus.z -= diff.x;

		}
		else {

			p1 = sel::calcPOnPlane(prevMouseRay, origin, planeNormal, pos);
			p2 = sel::calcPOnPlane(mouseRay, origin, planeNormal, pos);

			diff = p2 - p1;

			pos -= diff;
			focus -= diff;
		}

		prevMouseRay = mouseRay;

	}

	
}
}

