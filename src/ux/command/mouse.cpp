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
    if (mRightMouseSlot) {
        std::cout << "Right Mouse Click!\t";
        mRightMouseSlot->execute();
      }
}

void Mouse::leftMouseClick() {
    if (mLeftMouseSlot) {

#ifdef CADERA_DEBUG
        std::cout << "Left Mouse Click!\t";
#endif
        mLeftMouseSlot->execute();
      }


}

} // namespace command
}