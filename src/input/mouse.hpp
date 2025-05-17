#pragma once
#include "rendercommands.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

class Mouse {
public:

    Mouse();

    void setRightMouseSlot(Command* rightMouse);
    void setMiddleMouseSlot(Command* middleMouse);
    void setMiddleMouseReleaseSlot(Command* middleMouseRelease);
    void setScrollMouseSlot(Command* scrollMouse);
    void setLeftMouseSlot(Command* leftMouse);
    void setLeftMouseHoldSlot(Command* leftMouseHold);
    void setLeftMouseReleaseSlot(Command* leftMouseRelease);

    void rightMouseClick();

    void middleMouseClick();
    void middleMouseRelease();

    void scroll(double yOffset);

    void leftMouseClick();
    void leftMouseHold();
    void leftMouseRelease();


private:

    Command* mRightMouseSlot;
    Command* mMiddleMouseSlot;
    Command* mMiddleMouseReleaseSlot;
    Command* mScrollMouseSlot;
    Command* mLeftMouseSlot;
    Command* mLeftMouseHoldSlot;
    Command* mLeftMouseReleaseSlot;

};

}
}