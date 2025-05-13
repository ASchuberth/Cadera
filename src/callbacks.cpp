#include "callbacks.hpp"
#include "cadera.hpp"
#include "pch.hpp"

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {

  auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera *>(
      glfwGetWindowUserPointer(window));

  ImGuiIO &io = ImGui::GetIO();

  // (2) ONLY forward mouse data to your underlying app/game.
  if (!io.WantCaptureMouse) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
  
      app->Render.mouse.leftMouseClick();
   
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
  
      app->Render.mouse.leftMouseRelease();
    }
    

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {

      app->Render.Cam.flags.set(cad::cam::pan);
      app->Render.Cam.flags.set(cad::cam::mouseFirstPressed);
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {

      app->Render.Cam.flags.reset(cad::cam::pan);
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {

  auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera *>(
      glfwGetWindowUserPointer(window));

  if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_AnyWindow)) {
    app->Render.mouse.scroll(yoffset);
  }
}

void framebuffer_resize_callback(GLFWwindow *window, int width, int height) {

  auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera *>(
      glfwGetWindowUserPointer(window));

  app->Render.frameBufferResized = true;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {

  auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera *>(
      glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    app->Render.keyboard.escapePress();
  }

  if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
   
    app->Render.keyboard.deletePress();

  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
    app->Render.Sel.flags.set(CADERA_APP_NAMESPACE::sel::select_isCTRL);
  }

  if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
    app->Render.Sel.flags.reset(CADERA_APP_NAMESPACE::sel::select_isCTRL);
  }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {

  // Only run when left mouse button is being held
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) 
  {
     return;
  }

  auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera *>(
    glfwGetWindowUserPointer(window));

    app->Render.mouse.leftMouseHold();

}

