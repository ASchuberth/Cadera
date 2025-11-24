#pragma once
#include "keyboard.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

class Input {
public:

    Input();

    void setFramebufferResizeSlot(Command* framebufferResizeCmd);
    
    void framebufferResized();

    void setSketchCommands(sketch::Sketch* sketch, cam::Camera* camera, sel::Selector* selector);
    
    void setCameraCommands(cam::Camera* camera);

    void setRenderCommands(CADRender* render);

    void setMouse();

    void setKeyboard();

    void sketchMode(std::bitset<sketch::skt_num_flags>& sketchFlags);


    Keyboard keyboard;
    Mouse mouse;   

private:

    Command* mFramebufferResizeSlot;

    SketchAddPointCommand sketchAddPointCmd;
    SketchMovePointCommand sketchMovePointCmd;
    SketchSelectPointCommand sketchSelectPointCmd;
    SketchSelectSetCtrlCommand sketchSelectSetCtrlCmd;
    SketchSelectUnsetCtrlCommand sketchSelectUnsetCtrlCmd;
    SketchDeselectPointCommand sketchDeselectPointCmd;
    SketchDisableToolsCommand sketchDisableToolsCmd;
    SketchDeleteCommand sketchDeleteCmd;
    CameraZoomCommand cameraZoomCmd;
    CameraPanCommand cameraPanCmd;
    CameraUnsetPanCommand cameraUnsetPanCmd;
    RenderFramebufferResizeCommand renderFramebufferResizeCmd;

};

}
}