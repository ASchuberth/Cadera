#pragma once
#include "cameracommands.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

class Mouse {
public:

    Mouse();

    void setRightMouseSlot(Command* rightMouse);
    void setMiddleMouseSlot(Command* middleMouse);
    void setScrollMouseSlot(Command* scrollMouse);
    void setLeftMouseSlot(Command* leftMouse);

    void rightMouseClick();
    void scroll(double yOffset);
    void leftMouseClick();


private:

    Command* mRightMouseSlot;
    Command* mMiddleMouseSlot;
    Command* mScrollMouseSlot;
    Command* mLeftMouseSlot;

};

}
}