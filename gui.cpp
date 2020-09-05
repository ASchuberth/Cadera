#include "pch.hpp"
#include "gui.hpp"

namespace CADERA_APP_NAMESPACE {

namespace gui {

	

	void imguiBegin() {
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void imguiEnd() {
		ImGui::Render();
	}

	void showDebugWindow(CADRender & Render, sel::Selector& Sel) {

		//ImGui::SetNextWindowPos({ 0, 40 });
		ImGui::Begin("Debugging");


		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None))
		{

			static bool orthoCheck = false;
			ImGui::Checkbox("Orthogonal", &orthoCheck);


			if (orthoCheck && !Render.Cam.flags.test(cam::ortho)) {
				
				Render.Cam.flags.set(cam::ortho);
			
			}
			else if (!orthoCheck && Render.Cam.flags.test(cam::ortho)) {

				Render.Cam.flags.reset(cam::ortho);

			}
			
			ImGui::Text("Position");
			ImGui::Text("x: %f", Render.Cam.pos.x);
			ImGui::Text("y: %f", Render.Cam.pos.y);
			ImGui::Text("z: %f", Render.Cam.pos.z);

			ImGui::NewLine();

			ImGui::Text("Focus");
			ImGui::Text("x: %f", Render.Cam.focus.x);
			ImGui::Text("y: %f", Render.Cam.focus.y);
			ImGui::Text("z: %f", Render.Cam.focus.z);

			ImGui::NewLine();

			ImGui::Text("Mouse Ray");
			ImGui::Text("x: %f", Render.Cam.mouseRay.x);
			ImGui::Text("y: %f", Render.Cam.mouseRay.y);
			ImGui::Text("z: %f", Render.Cam.mouseRay.z);
	
	
		}

		if (ImGui::CollapsingHeader("Selection", ImGuiTreeNodeFlags_None)) {

			ImGui::Text("Slection Point");
			ImGui::Text("x: %f", Sel.point.x);
			ImGui::Text("y: %f", Sel.point.y);
			ImGui::Text("z: %f", Sel.point.z);

		}


		ImGui::End();


	}

	void imguiRun(CADRender & Render, sel::Selector& Sel) {

		// Cadera imgui begin, not part of Dear Imgui
		imguiBegin();

		ImGui::SetNextWindowPos({ 0, 20 });
		ImGui::SetNextWindowSize({ 300.0f,
								   static_cast<float>(Render.mMainCanvas.mExtent.height - 20) });

		ImGui::Begin("Main Window");

#ifdef CADERA_DEBUG
		static bool demoCheck = false;
		ImGui::Checkbox("Demo Window", &demoCheck);


		if (demoCheck) ImGui::ShowDemoWindow();

		static bool debugCheck = false;
		ImGui::Checkbox("Debug Window", &debugCheck);


		if (debugCheck) showDebugWindow(Render, Sel);


#endif

		ImGui::End();

		// Cadera imgui end, not part of Dear Imgui
		imguiEnd();


	}

}
}