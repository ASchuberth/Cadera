#pragma once
#include "keyboard.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

class Input {
public:

    Input();

    void setFramebufferResizeSlot(Command* framebufferResizeCmd);
    
    void framebufferResized();
    
    Keyboard keyboard;
    Mouse mouse;   

private:

    Command* mFramebufferResizeSlot;

};

}
}