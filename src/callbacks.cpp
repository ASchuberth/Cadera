#include "callbacks.hpp"
#include "cadera.hpp"
#include "pch.hpp"

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {

  auto input = reinterpret_cast<CADERA_APP_NAMESPACE::command::Input *>(
      glfwGetWindowUserPointer(window));

  ImGuiIO &io = ImGui::GetIO();

  // (2) ONLY forward mouse data to your underlying app/game.
  if (!io.WantCaptureMouse) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
  
      input->mouse.leftMouseClick();
   
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
  
      input->mouse.leftMouseRelease();
    }
    

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

      input->mouse.middleMouseClick();
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {

      input->mouse.middleMouseRelease();
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

  auto input = reinterpret_cast<CADERA_APP_NAMESPACE::command::Input *>(
      glfwGetWindowUserPointer(window));

  if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow)) {
    input->mouse.scroll(yoffset);
  }
}

void framebuffer_resize_callback(GLFWwindow *window, int width, int height) {

  auto input = reinterpret_cast<CADERA_APP_NAMESPACE::command::Input *>(
      glfwGetWindowUserPointer(window));

  input->framebufferResized();

}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  auto input = reinterpret_cast<CADERA_APP_NAMESPACE::command::Input *>(
      glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    input->keyboard.escapePress();
  }

  if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
   
    input->keyboard.deletePress();

  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
    input->keyboard.lCtrlPress();
  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
    input->keyboard.lCtrlRelease();
  }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {

  // Only run when left mouse button is being held
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) 
  {
     return;
  }

  auto input = reinterpret_cast<CADERA_APP_NAMESPACE::command::Input *>(
    glfwGetWindowUserPointer(window));

    input->mouse.leftMouseHold();

}

