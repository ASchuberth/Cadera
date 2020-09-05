#pragma once
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"
#include "CADRender.hpp"

namespace CADERA_APP_NAMESPACE {
namespace gui {

	enum GUIFlags {
		gui_start_menu,
		gui_sketch_menu,
		gui_num_flags
	};

	void imguiRun(CADRender &Render, sel::Selector& Sel);

	void imguiBegin();

	void imguiEnd();

	void startMenu(CADRender &Render, sel::Selector &Sel, std::bitset<gui_num_flags> &flags);

	void sketchMenu(CADRender &Render, sel::Selector &Sel, std::bitset<gui_num_flags> &flags);
	void showDebugWindow(CADRender &Render, sel::Selector &Sel, std::bitset<gui_num_flags> &flags);

}
}