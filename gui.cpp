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

	void startMenu(sketch::Sketch& Sketch, CADRender& Render, sel::Selector& Sel, 
		           std::bitset<gui_num_flags> &flags) {

		ImGui::Begin("Start Screen");

		if (ImGui::Button("New Sketch")) {
			
			flags.set(gui_sketch_menu);

			Sketch.setCameraDistance(&Render.Cam.camDistance);

			flags.reset(gui_start_menu);
		}

		ImGui::End();

	}

	void sketchMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel, 
		            std::bitset<gui_num_flags> &flags) {

		ImGui::SetNextWindowPos({ 0, 20 });
		ImGui::SetNextWindowSize({ 300.0f,
								   static_cast<float>(Render.mMainCanvas.mExtent.height - 20) });

		ImGui::Begin("Sketching");

#ifdef CADERA_DEBUG
		static bool demoCheck = false;
		ImGui::Checkbox("Demo Window", &demoCheck);


		if (demoCheck) ImGui::ShowDemoWindow();

		static bool debugCheck = false;
		ImGui::Checkbox("Debug Window", &debugCheck);


		if (debugCheck) showDebugWindow(Sketch, Render, Sel, flags);


#endif

		if (ImGui::Button("Point")) {
			Sketch.flags.set(sketch::skt_point_tool);
			Sketch.flags.set(sketch::skt_tool_active);
		}

		ImGui::End();

	}

	void showDebugWindow(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel, 
		                 std::bitset<gui_num_flags> &flags) {

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

			ImGui::NewLine();

			ImGui::Text("Camera Distance");
			ImGui::Text("Distance: %f", Render.Cam.camDistance);
	
	
	
		}

		if (ImGui::CollapsingHeader("Selection", ImGuiTreeNodeFlags_None)) {

			ImGui::Text("Slection Point");
			ImGui::Text("x: %f", Sel.point.x);
			ImGui::Text("y: %f", Sel.point.y);
			ImGui::Text("z: %f", Sel.point.z);

			ImGui::NewLine();

			ImGui::Text("Selected Points");
			for (const auto& p : Sel.selectedPoints) {
				ImGui::Text("Selected Point: %d", p.first);
				ImGui::Text("x: %f", p.second.pos.x);
				ImGui::Text("y: %f", p.second.pos.y);
				ImGui::Text("z: %f", p.second.pos.z);
				ImGui::NewLine();
			}

		}

		if (ImGui::CollapsingHeader("Sketch", ImGuiTreeNodeFlags_None)) {
			
			ImGui::Text("Sketch Tools:");
			ImGui::Text("Tool Active: %d", Sketch.flags.test(sketch::skt_tool_active));
			ImGui::Text("Point Tool Active: %d", Sketch.flags.test(sketch::skt_point_tool));
		
			ImGui::Text("Camera Distance");
			ImGui::Text("Distance: %f", *Sketch.mCamDistance);

			ImGui::NewLine();

			for (auto& p : Sketch.Points) {
				ImGui::Text("Sketch Point: %d", p.second.getId());
				ImGui::Text("x: %f", p.second.pos.x);
				ImGui::Text("y: %f", p.second.pos.y);
				ImGui::Text("z: %f", p.second.pos.z);
				ImGui::NewLine();
			}
		}

		ImGui::End();


	}

	void imguiRun(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel) {

		static std::bitset<gui_num_flags> flags(gui_start_menu + 1);
		


		// Cadera imgui begin, not part of Dear Imgui
		imguiBegin();

		
		if (flags.test(gui_start_menu))   startMenu(Sketch, Render, Sel, flags);
		if (flags.test(gui_sketch_menu))  sketchMenu(Sketch, Render, Sel, flags);

		


		// Cadera imgui end, not part of Dear Imgui
		imguiEnd();


	}

}
}