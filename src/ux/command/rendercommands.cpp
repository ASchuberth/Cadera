#include "rendercommands.hpp"
#include "pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace command {


// Camera Zoom Command
//-----------------------------------------------------------------------------
RenderFramebufferResizeCommand::RenderFramebufferResizeCommand() : mCamera{nullptr} {}


void RenderFramebufferResizeCommand::setRender(cam::Camera *cam) {
    mRender = cam;
}

void RenderFramebufferResizeCommand::execute(double yoffset) {
    if (mCamera) {

        mCamera->zoom(static_cast<float>(yoffset));

    }
}
//-----------------------------------------------------------------------------


}
}