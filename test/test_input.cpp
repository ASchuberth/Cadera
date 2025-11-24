#include <gtest/gtest.h>
#include "input/input.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

TEST(Input, inputFramebufferResized_Throw) {

    cad::command::Input input;


    EXPECT_ANY_THROW(input.framebufferResized());


}

TEST(Input, inputFramebufferResized_NoThrow) {

    cad::command::Input input;

    cad::CADRender render;

    cad::command::RenderFramebufferResizeCommand cmd;

    cmd.setRender(&render);

    input.setFramebufferResizeSlot(&cmd);

    EXPECT_NO_THROW(input.framebufferResized());
   

}