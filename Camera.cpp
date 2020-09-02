#include "pch.hpp"
#include "Camera.hpp"

namespace CADERA_APP_NAMESPACE {




	Camera::Camera()
	{
		 pos = { 20.0f, 0.0f, 0.0f };
		 cameraVec = { 1.0f, 0.0f, 0.0f };
	}

	void Camera::zoom(float yoffset) {

		if (yoffset > 0) {
			pos -= 1.0f * glm::normalize(cameraVec);
		}
		else if (yoffset < 0) {
			pos += 1.0f * glm::normalize(cameraVec);
		}

	}

}


