#pragma once
#include <glm/glm.hpp>

namespace CADERA_APP_NAMESPACE {

	class Camera {

	public:

		Camera();

		glm::vec3 pos;
		glm::vec3 cameraVec;

		void zoom(float yoffset);


	};

}

