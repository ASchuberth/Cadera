#include "pch.hpp"
#include "callbacks.hpp"
#include "Cadera.hpp"
//#include "Selection.hpp"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera*> (glfwGetWindowUserPointer(window));

	ImGuiIO& io = ImGui::GetIO();
    

    // (2) ONLY forward mouse data to your underlying app/game.
    if (!io.WantCaptureMouse) {

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow) &&
				app->Sketch.flags.test(CADERA_APP_NAMESPACE::sketch::skt_tool_active)) {
				
				app->Render.Sel.select(app->Render.Cam.mouseRay, glm::vec3(0.0f, 0.0f, 0.0f), 
									glm::vec3(1.0f, 0.0f, 0.0f), app->Render.Cam.pos,
									app->Render.Cam.flags.test(cad::cam::ortho));

				if (!app->Render.Sel.existingPoint(app->Render.Sel.point))
					app->Sketch.add(app->Render.Sel.point);

				app->Render.flags.set(CADERA_APP_NAMESPACE::render_update_sketch);
				
			}
			else if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow)) {
				
				app->Render.Sel.select(app->Render.Cam.mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
									glm::vec3(1.0f, 0.0f, 0.0f), app->Render.Cam.pos,
									app->Render.Cam.flags.test(cad::cam::ortho));

				int id = app->Render.Sel.add(app->Render.Sel.point, app->Sketch.Points, app->Render.Cam.camDistance);

				if (id >= 0 || app->Render.Sel.selectedPoints.empty()) {
					app->Render.flags.set(cad::render_update_sketch);
				}
				
			}
		}
	}
	

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

		app->Render.Cam.flags.set(cad::cam::pan);
		app->Render.Cam.flags.set(cad::cam::mouseFirstPressed);
		
	}

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {

		app->Render.Cam.flags.reset(cad::cam::pan);

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

	auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera*> (glfwGetWindowUserPointer(window));
	
	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow))
	app->Render.Cam.zoom(yoffset);
		

}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {

	auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera*> (glfwGetWindowUserPointer(window));


	app->Render.frameBufferResized = true;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera*> (glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		app->Sketch.deactivateTools();
	}

	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		app->flags.set(CADERA_APP_NAMESPACE::cadera_delete);
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
		app->Render.Sel.flags.set(CADERA_APP_NAMESPACE::sel::select_isCTRL);
	}

	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
		app->Render.Sel.flags.reset(CADERA_APP_NAMESPACE::sel::select_isCTRL);
	}

}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {

}

