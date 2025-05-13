#include "cadera.hpp"
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

  if (Sketch.flags.test(sketch::skt_tool_active)) {
    Render.mouse.setLeftMouseSlot(&sketchAddPointCmd);
    Render.mouse.setLeftMouseHoldSlot(nullptr);
    Render.mouse.setLeftMouseReleaseSlot(nullptr);
  } else {
    Render.mouse.setLeftMouseSlot(&sketchSelectPointCmd);
    Render.mouse.setLeftMouseHoldSlot(&sketchMovePointCmd);
    Render.mouse.setLeftMouseReleaseSlot(&sketchDeselectPointCmd);
  }

  // if (Sketch.flags.test(sketch::skt_move_points)) {

  //   Render.Sel.select(Render.Cam.mouseRay, glm::vec3(0.0f, 0.0f, 0.0f),
  //                     Render.Cam.cameraVec, Render.Cam.pos, Render.Cam.cross,
  //                     Render.Cam.flags.test(cad::cam::ortho));

  //   Sketch.movePoints(Render.Sel.selectedPoints, Render.Sel.point,
  //                     Render.Sel.flags.test(cad::sel::select_first_click));

  //   std::vector<int> ids = Render.Sel.getSelectedPointIds();

  //   std::map<int, Point> newPoints;

  //   for (const auto &id : ids) {
  //     newPoints[id] = Sketch.Points[id];
  //   }

  //   Render.Sel.update(newPoints);

  //   Render.Sel.flags.reset(cad::sel::select_first_click);

  //   Sketch.notify();
  // }

}

void Cadera::run() {

  Render.setBGColor(glm::vec4(0.3f, 0.3f, 0.3f, 0.1f));

  Render.Cam.flags.set(cam::ortho);
  Render.setup();

  initCallbacks();

  Render.initImgui();


  //Keyboard and Mouse Commands
  // TODO: Determine better way to handle commands
  sketchAddPointCmd.setSketch(&Sketch);
  sketchAddPointCmd.setSelector(&Render.Sel);
  sketchAddPointCmd.setCamera(&Render.Cam);

  sketchMovePointCmd.setSketch(&Sketch);
  sketchMovePointCmd.setSelector(&Render.Sel);
  sketchMovePointCmd.setCamera(&Render.Cam);

  sketchSelectPointCmd.setSketch(&Sketch);
  sketchSelectPointCmd.setSelector(&Render.Sel);
  sketchSelectPointCmd.setCamera(&Render.Cam);

  sketchDeselectPointCmd.setSketch(&Sketch);
  sketchDeselectPointCmd.setSelector(&Render.Sel);
  sketchDeselectPointCmd.setCamera(&Render.Cam);

  sketchDisableToolsCmd.setSketch(&Sketch);
  sketchDeleteCmd.setSketch(&Sketch);
  sketchDeleteCmd.setSelector(&Render.Sel);


  cameraZoomCmd.setCamera(&Render.Cam);


  
  Render.mouse.setScrollMouseSlot(&cameraZoomCmd);
  Render.mouse.setLeftMouseSlot(&sketchAddPointCmd);

  Render.keyboard.setEscapeSlot(&sketchDisableToolsCmd);
  Render.keyboard.setDeleteSlot(&sketchDeleteCmd);
  

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
