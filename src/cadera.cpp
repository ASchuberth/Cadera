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

void Cadera::run() {

  mRender.setBGColor(glm::vec4(0.3f, 0.3f, 0.3f, 0.1f));

  mRender.Cam.flags.set(cam::ortho);
  mRender.setup();

  initCallbacks();

  mRender.initImgui();

  mSelector.addRender(&mRender);

  //Keyboard and Mouse Commands
  input.setSketchCommands(&mSketch, &mRender.Cam, &mSelector);
  input.setCameraCommands(&mRender.Cam);
  input.setRenderCommands(&mRender);

  input.setMouse();
  input.setKeyboard();

 

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

    input.sketchMode(mSketch.flags);

    mRender.createCommandBuffers();
    mRender.drawFrame();
    mRender.runCamera();

    mRender.render();

    glfwWaitEvents();
  }
}


} // namespace CADERA_APP_NAMESPACE
