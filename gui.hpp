#pragma once
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"

namespace CADERA_APP_NAMESPACE {
namespace gui {


	void imguiRun(pcs::PecosRender const& Render);

	void imguiBegin();

	void imguiEnd();

}
}