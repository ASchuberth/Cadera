#pragma once
#include "mouse.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {

class Keyboard {
public:

    Keyboard();

    void setEscapeSlot(Command* escape);
    void setLCtrlSlot(Command* lCtrl);
    void setLCtrlReleaseSlot(Command* lCtrl);
    void setDeleteSlot(Command* del);


    void escapePress();
    void lCtrlPress();
    void lCtrlRelease();
    void deletePress();


private:

    Command* mEscapeSlot;
    Command* mLCtrlSlot;
    Command* mLCtrlReleaseSlot;
    Command* mDeleteSlot;


};

}
}