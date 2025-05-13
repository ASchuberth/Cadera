#pragma once
#include "sketchcommands.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


class RenderFramebufferResizeCommand : public Command { 
public:

    RenderFramebufferResizeCommand();

    void setCamera(cam::Camera* cam);

    void execute(double) override;

private:
    cam::Camera* mCamera;

};


}
}