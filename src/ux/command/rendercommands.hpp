#pragma once
#include "cameracommands.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


class RenderFramebufferResizeCommand : public Command { 
public:

    RenderFramebufferResizeCommand();

    void setRender(CADRender* render);

    void execute() override;

private:
    CADRender* mRender;

};


}
}