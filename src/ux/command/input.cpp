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
    mFramebufferResizeSlot->execute();
}


}
}