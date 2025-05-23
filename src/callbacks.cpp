#include "callbacks.hpp"
#include "cadera.hpp"
#include "pch.hpp"

void sketch_mode_callbacks(cad::Cadera *app, int &button, int &action,
                           int &mods);

void sketch_select_addPoint(cad::Cadera *app);

void sketch_select_removePoint(cad::Cadera *app);

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {

  auto app = reinterpret_cast<CADERA_APP_NAMESPACE::Cadera *>(
      glfwGetWindowUserPointer(window));

  ImGuiIO &io = ImGui::GetIO();

  // (2) ONLY forward mouse data to your underlying app/game.
  if (!io.WantCaptureMouse) {

    if (app->Sketch.flags.test(CADERA_APP_NAMESPACE::sketch::skt_active)) {

      sketch_mode_callbacks(app, button, action, mods);
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

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {}

void sketch_mode_callbacks(cad::Cadera *app, int &button, int &action,
                           int &mods) {

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (app->Sketch.flags.test(CADERA_APP_NAMESPACE::sketch::skt_tool_active)) {
      
      app->Render.mouse.leftMouseClick();

      app->Render.flags.set(CADERA_APP_NAMESPACE::render_update_sketch);
    }
    else {

      sketch_select_addPoint(app);
      app->Sketch.flags.set(cad::sketch::skt_move_points);
    }

  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

    if (!app->Sketch.flags.test(
            CADERA_APP_NAMESPACE::sketch::skt_tool_active)) {

      if (!app->Sketch.flags.test(cad::sketch::skt_points_have_moved))
        sketch_select_removePoint(app);

      app->Render.Sel.flags.set(cad::sel::select_first_click);
      app->Sketch.flags.reset(cad::sketch::skt_move_points);
      app->Sketch.flags.reset(cad::sketch::skt_points_have_moved);
    }
  }
}

void sketch_select_addPoint(cad::Cadera *app) {

  app->Render.Sel.select(app->Render.Cam.mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                         app->Render.Cam.cameraVec, app->Render.Cam.pos,
                         app->Render.Cam.cross,
                         app->Render.Cam.flags.test(cad::cam::ortho));

  int id = app->Render.Sel.add(app->Render.Sel.point, app->Sketch.Points,
                               app->Render.Cam.camDistance);

  if (id >= 0 || app->Render.Sel.selectedPoints.empty()) {
    app->Render.flags.set(cad::render_update_sketch);
  }
}

void sketch_select_removePoint(cad::Cadera *app) {

  app->Render.Sel.select(app->Render.Cam.mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                         app->Render.Cam.cameraVec, app->Render.Cam.pos,
                         app->Render.Cam.cross,
                         app->Render.Cam.flags.test(cad::cam::ortho));

  int id = app->Render.Sel.remove(app->Render.Sel.point, app->Sketch.Points,
                                  app->Render.Cam.camDistance);

  if (id >= 0 || app->Render.Sel.selectedPoints.empty()) {
    app->Render.flags.set(cad::render_update_sketch);
  }
}