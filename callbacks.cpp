#include "pch.hpp"
#include "callbacks.hpp"
#include "CADRender.hpp"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));
	std::cout << "click" << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	auto app = reinterpret_cast<cad::CADRender*> (glfwGetWindowUserPointer(window));
	std::cout << "scroll" << std::endl;
	app->runCamera(yoffset);

}

