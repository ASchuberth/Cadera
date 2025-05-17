#include "cadera.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

void Cadera::initCallbacks() {

  glfwSetWindowUserPointer(mRender.mMainWindow, &this->input);

  glfwSetMouseButtonCallback(mRender.mMainWindow, mouse_button_callback);
  glfwSetScrollCallback(mRender.mMainWindow, scroll_callback);
  glfwSetFramebufferSizeCallback(mRender.mMainWindow,
                                 framebuffer_resize_callback);
  glfwSetCursorPosCallback(mRender.mMainWindow, cursor_position_callback);
  glfwSetKeyCallback(mRender.mMainWindow, key_callback);
}

Cadera::Cadera() {
  modelIdCounter = 0;
  // Sketch.setType(cad_sketch);
}

Cadera::~Cadera() { mRender.destroy(); }

void Cadera::SketchEvents() {

  if (mSketch.flags.test(sketch::skt_tool_active)) {
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

  mRender.setBGColor(glm::vec4(0.3f, 0.3f, 0.3f, 0.1f));

  mRender.Cam.flags.set(cam::ortho);
  mRender.setup();

  initCallbacks();

  mRender.initImgui();


  //Keyboard and Mouse Commands
  // TODO: Determine better way to handle commands
  sketchAddPointCmd.setSketch(&mSketch);
  sketchAddPointCmd.setSelector(&mSelector);
  sketchAddPointCmd.setCamera(&mRender.Cam);

  sketchMovePointCmd.setSketch(&mSketch);
  sketchMovePointCmd.setSelector(&mSelector);
  sketchMovePointCmd.setCamera(&mRender.Cam);

  sketchSelectPointCmd.setSketch(&mSketch);
  sketchSelectPointCmd.setSelector(&mSelector);
  sketchSelectPointCmd.setCamera(&mRender.Cam);

  sketchDeselectPointCmd.setSketch(&mSketch);
  sketchDeselectPointCmd.setSelector(&mSelector);
  sketchDeselectPointCmd.setCamera(&mRender.Cam);

  sketchSelectSetCtrlCmd.setSelector(&mSelector);
  sketchSelectUnsetCtrlCmd.setSelector(&mSelector);

  sketchDisableToolsCmd.setSketch(&mSketch);
  sketchDeleteCmd.setSketch(&mSketch);
  sketchDeleteCmd.setSelector(&mSelector);


  cameraZoomCmd.setCamera(&mRender.Cam);
  cameraPanCmd.setCamera(&mRender.Cam);
  cameraUnsetPanCmd.setCamera(&mRender.Cam);

  renderFramebufferResizeCmd.setRender(&mRender);


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

  while (!glfwWindowShouldClose(mRender.mMainWindow)) {

    glfwPollEvents();

    gui::imguiRun(mSketch, mRender, mSelector);

    SketchEvents();

    mRender.createCommandBuffers();
    mRender.drawFrame();
    mRender.runCamera();

    glfwWaitEvents();
  }
}


} // namespace CADERA_APP_NAMESPACE
