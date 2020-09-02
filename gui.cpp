#include "pch.hpp"
#include "gui.hpp"

namespace CADERA_APP_NAMESPACE {

namespace gui {

	void imguiRun(pcs::PecosRender const& Render) {

		// Cadera imgui begin, not part of Dear Imgui
		imguiBegin();

		ImGui::SetNextWindowPos({ 0, 20 });
		ImGui::SetNextWindowSize({ 300.0f,
								   static_cast<float>(Render.mMainCanvas.mExtent.height - 20) });

		ImGui::Begin("Main Window");

#ifdef CADERA_DEBUG
		static bool check = false;
		ImGui::Checkbox("Demo Window", &check);


		if (check) ImGui::ShowDemoWindow();
#endif

		ImGui::End();

		// Cadera imgui end, not part of Dear Imgui
		imguiEnd();


	}

	void imguiBegin() {
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void imguiEnd() {
		ImGui::Render();
	}

}
}