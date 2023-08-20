#pragma once
#include "Selection.hpp"

namespace CADERA_APP_NAMESPACE {

namespace cam {
	
	enum CameraFlags {
		/// Toggle orthographics projection
		ortho,                       
		/// Used for resetting functions such as pan() on first mouse click
		mouseFirstPressed,           
		/// Toggle the camera pan() function
		pan,                         
		/// Bitset size
		camera_number_flags                     
	};

	/**
	 * @brief 
	 * 
	 * 
	 */
	class Camera {

	public:

		Camera();

		std::bitset<camera_number_flags> flags;
		
		/// Position of the camera in 3D space
		glm::vec3 pos; 
		/// Focus point of the camera in 3D space
		glm::vec3 focus; 
		/// The vector going from pos to focus
		glm::vec3 cameraVec;  
		/// The length of the cameraVec vector
		float camDistance;  

		/// Up direction of Camera
		glm::vec3 up;

		/// Direction to move Camera to the side, for panning
		glm::vec3 cross;

		// Ortho view
		float left;
	

		// For mouse input and selection
		glm::vec3 mouseRay;

		// For obtaining mouse position on screen
		double xpos;
		double ypos;

        void setXYView();

        void setYZView();

        void setZXView();

        void update();

        /**
		 * @brief 
		 * 
		 * @param yoffset 
		 */
		void zoom(float yoffset);

		void updateMouseRay(float x, float y, glm::mat4 modelMat, glm::mat4 viewMat, glm::mat4 projMat, uint32_t width,
			                uint32_t height);

		void pan(glm::vec3 origin, glm::vec3 planeNormal);
		

	};

}
}
