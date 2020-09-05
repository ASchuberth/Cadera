#include "pch.hpp"
#include "callbacks.hpp"
#include "CADRender.hpp"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow)) {
			app->Sel.select(app->Cam.mouseRay, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), app->Cam.pos,
				app->Cam.flags.test(cad::cam::ortho));
		}
	}
	

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

		app->Cam.flags.set(cad::cam::pan);
		app->Cam.flags.set(cad::cam::mouseFirstPressed);
		
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {

		app->Cam.flags.reset(cad::cam::pan);

	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));
	
	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow))
		app->runCameraScroll(yoffset);

}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));


	app->mMainCanvas.frameBufferResized = true;

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

}

