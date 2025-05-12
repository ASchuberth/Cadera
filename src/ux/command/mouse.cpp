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

} // namespace command
}