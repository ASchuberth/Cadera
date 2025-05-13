#include "mouse.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

Mouse::Mouse() : 
    mRightMouseSlot{nullptr}, 
    mMiddleMouseSlot{nullptr},
    mScrollMouseSlot{nullptr}, 
    mLeftMouseSlot{nullptr} {}

void Mouse::setRightMouseSlot(Command *rightMouse) {
    mRightMouseSlot = rightMouse;
}

void Mouse::setMiddleMouseSlot(Command *middleMouse) {
    mMiddleMouseSlot = middleMouse;
}

void Mouse::setScrollMouseSlot(Command *scrollMouse) {
    mScrollMouseSlot = scrollMouse;
}

void Mouse::setLeftMouseSlot(Command *leftMouse) {
    mLeftMouseSlot = leftMouse;
}

void Mouse::setLeftMouseHoldSlot(Command * leftMouseHold)
{
    mLeftMouseHoldSlot = leftMouseHold;
}

void Mouse::setLeftMouseReleaseSlot(Command *leftMouseRelease) {
    mLeftMouseReleaseSlot = leftMouseRelease;
}

void Mouse::rightMouseClick() {
    if (mRightMouseSlot) 
        mRightMouseSlot->execute();
      
}

void Mouse::scroll(double yOffset) {
    if (mScrollMouseSlot)
        mScrollMouseSlot->execute(yOffset);
}


void Mouse::leftMouseClick() {
    if (mLeftMouseSlot) 
        mLeftMouseSlot->execute();
      
}

void Mouse::leftMouseHold() {
    if (mLeftMouseHoldSlot) {
        mLeftMouseHoldSlot->execute();
    }
}

void Mouse::leftMouseRelease() {
    if (mLeftMouseReleaseSlot) {
        mLeftMouseReleaseSlot->execute();
    }
}

} // namespace command
}