#pragma once
#include "Selection.hpp"

namespace CADERA_APP_NAMESPACE {

namespace cam {
	
	enum CameraFlags {
		ortho,                       // Toggle orthographics projection
		mouseFirstPressed,           // Used for resetting functions such as pan() on first mouse click
		pan,                         // Toggle the camera pan() function
		numFlags                     // Bitset size
	};


	class Camera {

	public:

		Camera();

		std::bitset<numFlags> flags;

		glm::vec3 pos;
		glm::vec3 focus;
		glm::vec3 cameraVec;
		float camDistance;

		// Ortho view
		float left;
	

		// For mouse input and selection
		glm::vec3 mouseRay;

		// For obtaining mouse position on screen
		double xpos;
		double ypos;

		void update();

		void zoom(float yoffset);

		void updateMouseRay(float x, float y, glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width,
			                uint32_t height);

		void pan(glm::vec3 origin, glm::vec3 planeNormal);
		

	};

}
}
