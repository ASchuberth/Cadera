#include "mouse.hpp"
#include "pch.hpp"
#include "keyboard.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

Keyboard::Keyboard() : 
    mEscapeSlot{nullptr}, 
    mLCtrlSlot{nullptr},
    mDeleteSlot{nullptr} {}


void Keyboard::setEscapeSlot(Command * escape)
{
    mEscapeSlot = escape;
}

void Keyboard::setLCtrlSlot(Command * lCtrl)
{
    mLCtrlSlot = lCtrl;
}

void Keyboard::setDeleteSlot(Command * del)
{
    mDeleteSlot = del;
}

void Keyboard::escapePress()
{
    if (mEscapeSlot) {
        mEscapeSlot->execute();
    }
}

void Keyboard::lCtrlPress()
{
}
void Keyboard::deletePress()
{
}

} // namespace command
}