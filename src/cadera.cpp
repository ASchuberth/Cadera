#include "cadera.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

void Cadera::initCallbacks() {

  glfwSetWindowUserPointer(Render.mMainWindow, &this->input);

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
    input.mouse.setLeftMouseSlot(&sketchAddPointCmd);
    input.mouse.setLeftMouseHoldSlot(nullptr);
    input.mouse.setLeftMouseReleaseSlot(nullptr);
  } else {
    input.mouse.setLeftMouseSlot(&sketchSelectPointCmd);
    input.mouse.setLeftMouseHoldSlot(&sketchMovePointCmd);
    input.mouse.setLeftMouseReleaseSlot(&sketchDeselectPointCmd);
  }

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

  sketchSelectSetCtrlCmd.setSelector(&Render.Sel);
  sketchSelectUnsetCtrlCmd.setSelector(&Render.Sel);

  sketchDisableToolsCmd.setSketch(&Sketch);
  sketchDeleteCmd.setSketch(&Sketch);
  sketchDeleteCmd.setSelector(&Render.Sel);


  cameraZoomCmd.setCamera(&Render.Cam);

  cameraPanCmd.setCamera(&Render.Cam);

  cameraUnsetPanCmd.setCamera(&Render.Cam);

  renderFramebufferResizeCmd.setRender(&Render);


  input.mouse.setScrollMouseSlot(&cameraZoomCmd);
  input.mouse.setLeftMouseSlot(&sketchAddPointCmd);
  input.mouse.setMiddleMouseSlot(&cameraPanCmd);
  input.mouse.setMiddleMouseReleaseSlot(&cameraUnsetPanCmd);

  input.keyboard.setEscapeSlot(&sketchDisableToolsCmd);
  input.keyboard.setDeleteSlot(&sketchDeleteCmd);
  input.keyboard.setLCtrlSlot(&sketchSelectSetCtrlCmd);
  input.keyboard.setLCtrlReleaseSlot(&sketchSelectUnsetCtrlCmd);
  
  input.setFramebufferResizeSlot(&renderFramebufferResizeCmd);

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
