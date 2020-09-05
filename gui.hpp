#pragma once
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"
#include "CADRender.hpp"

namespace CADERA_APP_NAMESPACE {
namespace gui {

	
	void imguiRun(CADRender &Render, sel::Selector& Sel);

	void imguiBegin();

	void imguiEnd();

	void showDebugWindow(CADRender &Render, sel::Selector &Sel);

}
}