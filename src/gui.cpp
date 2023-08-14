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


	void mainMenuBar() {
	
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				ImGui::MenuItem("New");
				ImGui::MenuItem("Open");
				ImGui::MenuItem("Save");
				ImGui::MenuItem("Close");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {

				ImGui::MenuItem("Undo");

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools")) {

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help")) {

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}

	void startMenu(sketch::Sketch& Sketch, CADRender& Render, sel::Selector& Sel, 
		           std::bitset<gui_num_flags> &flags) {

		ImGui::Begin("Start Screen");

		if (ImGui::Button("New Sketch")) {
			
			flags.set(gui_sketch_menu);

			Sketch.setType(cad_sketch);
			Sketch.setCameraDistance(&Render.Cam.camDistance);
			Sel.setActiveSketch(&Sketch);

			Render.flags.set(render_update_sketch);

			flags.reset(gui_start_menu);
		}

		ImGui::End();

	}

	void sketchMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel, 
		            std::bitset<gui_num_flags> &flags) {

		ImGui::SetNextWindowPos({ 0, 20 });
		ImGui::SetNextWindowSize({ 300.0f,
								   static_cast<float>(Render.mExtent.height - 20) });

		ImGui::Begin("Sketching");

#ifdef CADERA_DEBUG
		static bool demoCheck = false;
		ImGui::Checkbox("Demo Window", &demoCheck);


		if (demoCheck) ImGui::ShowDemoWindow();

		static bool debugCheck = true;
		ImGui::Checkbox("Debug Window", &debugCheck);


		if (debugCheck) showDebugWindow(Sketch, Render, Sel, flags);


#endif

		if (ImGui::Button("Grid Options")) {

			flags.set(gui_sketch_grid_menu);
		}

		if (ImGui::Button("Point")) {

			Sketch.activatePointTool();

		}

		if (ImGui::Button("Note")) {

			Sketch.activateNoteTool();
			
		}

		if (Sketch.flags.test(sketch::skt_note_tool)) {
			

			static ImGuiInputTextFlags flags = {};
			
			ImGui::InputTextMultiline("Note Text", &Sketch.noteText );
		}

		if (ImGui::CollapsingHeader("Relations", ImGuiTreeNodeFlags_DefaultOpen)) {

			if (Sel.flags.test(sel::select_double_point) || Sel.flags.test(sel::select_multi_point)) {

				if (ImGui::Button("Vertical")) {
				
					bool success = Render.SktSolver.addNewSketchRelation(Sel.getSelectedPointIds(), cad::rel_vertical);
					
					if (success) 
						Render.flags.set(render_update_sketch);
					
				}

				if (ImGui::Button("Horizontal")) {
					
					bool success = Render.SktSolver.addNewSketchRelation(Sel.getSelectedPointIds(), cad::rel_horizontal);

					if (success)
						Render.flags.set(render_update_sketch);

				}

				if (ImGui::Button("Coincident")) {

					bool success = Render.SktSolver.addNewSketchRelation(Sel.getSelectedPointIds(), cad::rel_coincident);

					if (success)
						Render.flags.set(render_update_sketch);

				}

				if (ImGui::Button("Clear")) {

					Sketch.clearRelations();

					Render.flags.set(render_update_sketch);

				}

			}

		}


		ImGui::End();

	}

	void gridMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel, 
		            std::bitset<gui_num_flags> &flags) {

		ImGui::SetNextWindowPos({ 320, 20 });
		ImGui::SetNextWindowSize({ 300.0f, 300.0f });

		ImGui::Begin("Grid Options Menu");

		ImGui::InputInt("Grid Size", &Sketch.mGrid.size);
		ImGui::InputFloat("Grid Spacing", &Sketch.mGrid.spacing);

		if (ImGui::Button("Close")) {
			flags.reset(gui_sketch_grid_menu);
		}

		ImGui::End();

	}

	void showDebugWindow(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel, 
		                 std::bitset<gui_num_flags> &flags) {


		ImGui::Begin("Debugging");


		if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None))
		{

			static bool orthoCheck = true;
			ImGui::Checkbox("Orthogonal", &orthoCheck);


			if (orthoCheck && !Render.Cam.flags.test(cam::ortho)) {
				
				Render.Cam.flags.set(cam::ortho);
			
			}
			else if (!orthoCheck && Render.Cam.flags.test(cam::ortho)) {

				Render.Cam.flags.reset(cam::ortho);

			}
			

			ImGui::Text("Left: %f", Render.Cam.left);

			ImGui::NewLine();

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

			ImGui::Text("Selection Flags");
			ImGui::Text("Single Point: %d", Render.Sel.flags.test(sel::select_single_point));
			ImGui::Text("Double Point: %d", Render.Sel.flags.test(sel::select_double_point));
			ImGui::Text("Multi Point: %d", Render.Sel.flags.test(sel::select_multi_point));

			ImGui::NewLine();

			ImGui::Text("Selection Point");
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

		if (ImGui::CollapsingHeader("Sketch", ImGuiTreeNodeFlags_DefaultOpen)) {
			
			ImGui::Text("Sketch Tools:");
			ImGui::Text("Tool Active: %d", Sketch.flags.test(sketch::skt_tool_active));
			ImGui::Text("Point Tool Active: %d", Sketch.flags.test(sketch::skt_point_tool));
			ImGui::Text("Note Tool Active: %d", Sketch.flags.test(sketch::skt_note_tool));
			ImGui::Text("FeatureCounter: %d", Sketch.getFeatureCounter());
		
			ImGui::Text("Camera Distance");
			ImGui::Text("Distance: %f", *Sketch.mCamDistance);

			ImGui::NewLine();
			
			for (auto& rel : Sketch.mRelations) {

				if (rel.second.mId == 0)
					continue;

				if (rel.second.mType == cad::rel_vertical)
					ImGui::Text("Relation: Vertical");
				if (rel.second.mType == cad::rel_horizontal)
					ImGui::Text("Relation: Horizontal");
				if (rel.second.mType == cad::rel_coincident)
					ImGui::Text("Relation: Coincident");


				ImGui::Text("Id: %d", rel.second.mId);

				ImGui::Text("Features:");
				for (const auto& id : rel.second.mFeatureIds) {
					ImGui::Text("%d", id);
				}
				
			}
			
			ImGui::NewLine();

			for (auto& p : Sketch.Points) {
				ImGui::Text("Sketch Point: %d", p.second.getId());
				
				if (p.second.Type == feat_point)
					ImGui::Text("Type: Point");

				ImGui::Text("x: %f", p.second.pos.x);
				ImGui::Text("y: %f", p.second.pos.y);
				ImGui::Text("z: %f", p.second.pos.z);
				ImGui::NewLine();

				ImGui::Text("Relation Ids:");
				for (const auto& id : p.second.relationIds) {
					ImGui::Text("%d", id);
				}
				ImGui::NewLine();

			}
		}

		ImGui::End();


	}

	void imguiRun(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel) {

		static std::bitset<gui_num_flags> flags(gui_start_menu + 1);
		


		// Cadera imgui begin, not part of Dear Imgui
		imguiBegin();

		mainMenuBar();
		
		if (flags.test(gui_start_menu))   startMenu(Sketch, Render, Sel, flags);
		if (flags.test(gui_sketch_menu))  sketchMenu(Sketch, Render, Sel, flags);
		if (flags.test(gui_sketch_grid_menu))  gridMenu(Sketch, Render, Sel, flags);

		


		// Cadera imgui end, not part of Dear Imgui
		imguiEnd();


	}

}
}