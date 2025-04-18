#include "Cadera.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

void Cadera::initCallbacks() {

  glfwSetWindowUserPointer(Render.mMainWindow, this);

  glfwSetMouseButtonCallback(Render.mMainWindow, mouse_button_callback);
  glfwSetScrollCallback(Render.mMainWindow, scroll_callback);
  glfwSetFramebufferSizeCallback(Render.mMainWindow,
                                 framebuffer_resize_callback);
  glfwSetCursorPosCallback(Render.mMainWindow, cursor_position_callback);
  glfwSetKeyCallback(Render.mMainWindow, key_callback);
}

Cadera::Cadera() {
  modelIdCounter = 0;
  // Sketch.setType(cad_sketch);
}

Cadera::~Cadera() { Render.destroy(); }

void Cadera::SketchEvents() {

  if (flags.test(cadera_delete) && !Render.Sel.selectedPoints.empty()) {
    Sketch.deletion(Render.Sel.getSelectedPointIds());
    Render.Sel.clear();
    Render.Sel.setFlags();
    Render.flags.set(render_update_sketch);
    flags.reset(cadera_delete);
  } else {
    flags.reset(cadera_delete);
  }

  if (Render.flags.test(render_update_sketch)) {
    Render.render(Sketch);
  }

  if (Sketch.flags.test(sketch::skt_move_points)) {

    Render.Sel.select(Render.Cam.mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
                      Render.Cam.cameraVec, Render.Cam.pos, Render.Cam.cross,
                      Render.Cam.flags.test(cad::cam::ortho));

    Sketch.movePoints(Render.Sel.selectedPoints, Render.Sel.point,
                      Render.Sel.flags.test(cad::sel::select_first_click));

    std::vector<int> ids = Render.Sel.getSelectedPointIds();

    std::map<int, Point> newPoints;

    for (const auto &id : ids) {
      newPoints[id] = Sketch.Points[id];
    }

    Render.Sel.update(newPoints);

    Render.Sel.flags.reset(cad::sel::select_first_click);

    Render.flags.set(render_update_sketch);
  }

  if (Sketch.flags.test(sketch::skt_point_tool)) {

    Render.renderSketchPointTool();
  }

  if (Sketch.flags.test(sketch::skt_event_tool_deactivated)) {
    Render.deleteBuffer(BUF_SKETCH_POINT_TOOL);
    Sketch.flags.reset(sketch::skt_event_tool_deactivated);
  }
}

void Cadera::run() {

  Render.setBGColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));

  Render.Cam.flags.set(cam::ortho);
  Render.setup();

  initCallbacks();

  Render.initImgui();

  Render.SktSolver.setActiveSketch(&Sketch);

  mainLoop();
}

/**
 * @brief Main loop of the Cadera App
 *
 *
 */
void Cadera::mainLoop() {

  while (!glfwWindowShouldClose(Render.mMainWindow)) {

    glfwPollEvents();

    gui::imguiRun(Sketch, Render, Render.Sel);

    SketchEvents();

    Render.createCommandBuffers();
    Render.drawFrame();
    Render.runCamera();

    glfwWaitEvents();
  }
}

} // namespace CADERA_APP_NAMESPACE