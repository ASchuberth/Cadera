#include "rendercommands.hpp"
#include "pch.hpp"


namespace CADERA_APP_NAMESPACE {

namespace command {


// Render Framebuffer Resize Command
//-----------------------------------------------------------------------------
RenderFramebufferResizeCommand::RenderFramebufferResizeCommand() : mRender{nullptr} {}

void RenderFramebufferResizeCommand::setRender(CADRender * render) {
    mRender = render;
}




void RenderFramebufferResizeCommand::execute() {
    if (mRender) {

        mRender->frameBufferResized = true;;

    }
}
//-----------------------------------------------------------------------------


}
}