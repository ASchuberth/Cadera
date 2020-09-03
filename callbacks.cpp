#include "pch.hpp"
#include "callbacks.hpp"
#include "CADRender.hpp"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

		app->Cam.flags.set(cad::CAM_PAN);
		app->Cam.flags.set(cad::CAM_MOUSE_FIRST_PRESS);
		
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {

		app->Cam.flags.reset(cad::CAM_PAN);

	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));
	app->runCameraScroll(yoffset);

}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));


	app->mMainCanvas.frameBufferResized = true;

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

}

