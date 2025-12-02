#include "input.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

Input::Input() : mFramebufferResizeSlot{nullptr} {

}

void Input::setFramebufferResizeSlot(Command* framebufferResizeCmd) {
    mFramebufferResizeSlot = framebufferResizeCmd;
}

void Input::framebufferResized()
{

    if (mFramebufferResizeSlot) {
        mFramebufferResizeSlot->execute();
    }
    else {
        throw std::runtime_error("Input Framebuffer Resize Slot not valid!\n");
    }
}

void Input::setSketchCommands(sketch::Sketch *sketch, cam::Camera *camera,
                              sel::Selector *selector) {


    sketchAddPointCmd.setSketch(sketch);
    sketchAddPointCmd.setSelector(selector);
    sketchAddPointCmd.setCamera(camera);
    sketchMovePointCmd.setSketch(sketch);
    sketchMovePointCmd.setSelector(selector);
    sketchMovePointCmd.setCamera(camera);
    sketchSelectPointCmd.setSketch(sketch);
    sketchSelectPointCmd.setSelector(selector);
    sketchSelectPointCmd.setCamera(camera);
    sketchDeselectPointCmd.setSketch(sketch);
    sketchDeselectPointCmd.setSelector(selector);
    sketchDeselectPointCmd.setCamera(camera);
    sketchSelectSetCtrlCmd.setSelector(selector);
    sketchSelectUnsetCtrlCmd.setSelector(selector);
    sketchDisableToolsCmd.setSketch(sketch);
    sketchDeleteCmd.setSketch(sketch);
    sketchDeleteCmd.setSelector(selector);

}


void Input::setCameraCommands(cam::Camera *camera) {


  cameraZoomCmd.setCamera(camera);
  cameraPanCmd.setCamera(camera);
  cameraUnsetPanCmd.setCamera(camera);

}

void Input::setRenderCommands(CADRender* render) {

    renderFramebufferResizeCmd.setRender(render);

    mFramebufferResizeSlot = &renderFramebufferResizeCmd;
}

void Input::setMouse() {

    mouse.setScrollMouseSlot(&cameraZoomCmd);
    mouse.setLeftMouseSlot(&sketchAddPointCmd);
    mouse.setMiddleMouseSlot(&cameraPanCmd);
    mouse.setMiddleMouseReleaseSlot(&cameraUnsetPanCmd);
}

void Input::setKeyboard() {

    keyboard.setEscapeSlot(&sketchDisableToolsCmd);
    keyboard.setDeleteSlot(&sketchDeleteCmd);
    keyboard.setLCtrlSlot(&sketchSelectSetCtrlCmd);
    keyboard.setLCtrlReleaseSlot(&sketchSelectUnsetCtrlCmd);

}

void Input::sketchMode(std::bitset<sketch::skt_num_flags> &sketchFlags) {


    if (sketchFlags.test(sketch::skt_tool_active)) {
        mouse.setLeftMouseSlot(&sketchAddPointCmd);
        mouse.setLeftMouseHoldSlot(nullptr);
        mouse.setLeftMouseReleaseSlot(nullptr);
    } else {
        mouse.setLeftMouseSlot(&sketchSelectPointCmd);
        mouse.setLeftMouseHoldSlot(&sketchMovePointCmd);
        mouse.setLeftMouseReleaseSlot(&sketchDeselectPointCmd);
    }
    
}

}
}