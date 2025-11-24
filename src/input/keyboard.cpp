#include "mouse.hpp"
#include "pch.hpp"
#include "keyboard.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

Keyboard::Keyboard() : 
    mEscapeSlot{nullptr}, 
    mLCtrlSlot{nullptr},
    mLCtrlReleaseSlot{nullptr},
    mDeleteSlot{nullptr} {}


void Keyboard::setEscapeSlot(Command * escape)
{
    mEscapeSlot = escape;
}

void Keyboard::setLCtrlSlot(Command * lCtrl)
{
    mLCtrlSlot = lCtrl;
}

void Keyboard::setLCtrlReleaseSlot(Command * lCtrl)
{
    mLCtrlReleaseSlot = lCtrl;
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
     if (mLCtrlSlot) {
        mLCtrlSlot->execute();
    }
}
void Keyboard::lCtrlRelease()
{
    if (mLCtrlReleaseSlot) {
        mLCtrlReleaseSlot->execute();
    }
}
void Keyboard::deletePress()
{
    if (mDeleteSlot) {
        mDeleteSlot->execute();
    }
}

} // namespace command
}