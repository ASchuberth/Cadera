#include "gui.hpp"
#include "../pch.hpp"

namespace CADERA_APP_NAMESPACE {

namespace gui {

void imguiBegin() {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void imguiEnd() { ImGui::Render(); }

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

void startMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel,
               std::bitset<gui_num_flags> &flags) {

  ImGui::Begin("Start Screen");

  static int modelTypeInt;
  ImGui::RadioButton("Sketch", &modelTypeInt, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Part", &modelTypeInt, 1);

  if (modelTypeInt == 0) {

    ImGui::Text("Sketch Plane:");

    static int sketchViewInt;
    ImGui::RadioButton("XY", &sketchViewInt, 0);
    ImGui::SameLine();
    ImGui::RadioButton("ZX", &sketchViewInt, 1);
    ImGui::SameLine();
    ImGui::RadioButton("YZ", &sketchViewInt, 2);

    if (ImGui::Button("New Sketch")) {

      flags.set(gui_sketch_menu);

      if (sketchViewInt == 0) {
        Render.Cam.setXYView();
        Sketch.mGrid.setGridOrientation(Render.Cam.cameraVec, Render.Cam.up,
                                        Render.Cam.cross);
      } else if (sketchViewInt == 1) {
        Render.Cam.setZXView();
        Sketch.mGrid.setGridOrientation(Render.Cam.cameraVec, Render.Cam.up,
                                        Render.Cam.cross);
      } else {
        Render.Cam.setYZView();
        Sketch.mGrid.setGridOrientation(Render.Cam.cameraVec, Render.Cam.up,
                                        Render.Cam.cross);
      }

      Sketch.flags.set(sketch::skt_active);
      Sketch.setType(cad_sketch);
      Sketch.setCameraDistance(&Render.Cam.camDistance);
      Sketch.addRender(&Render);
      Sel.setActiveSketch(&Sketch);

      Sketch.notify();

      flags.reset(gui_start_menu);
    }
  }

  ImGui::End();
}

void sketchMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel,
                std::bitset<gui_num_flags> &flags) {

  ImGui::SetNextWindowPos({0, 20});
  ImGui::SetNextWindowSize(
      {300.0f, static_cast<float>(Render.mExtent.height - 20)});

  ImGui::Begin("Sketching");

#ifdef CADERA_DEBUG
  static bool demoCheck = false;
  ImGui::Checkbox("Demo Window", &demoCheck);

  if (demoCheck)
    ImGui::ShowDemoWindow();

  static bool debugCheck = true;
  ImGui::Checkbox("Debug Window", &debugCheck);

  if (debugCheck)
    showDebugWindow(Sketch, Render, Sel, flags);

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

    ImGui::InputTextMultiline("Note Text", &Sketch.noteText);
  }

  if (ImGui::CollapsingHeader("Relations", ImGuiTreeNodeFlags_DefaultOpen)) {

    if (Sel.flags.test(sel::select_double_point) ||
        Sel.flags.test(sel::select_multi_point)) {

      if (ImGui::Button("Vertical")) {

        bool success = Render.SktSolver.addNewSketchRelation(
            Sel.getSelectedPointIds(), cad::rel_vertical);

        if (success)
          Render.flags.set(render_update_sketch);
      }

      if (ImGui::Button("Horizontal")) {

        bool success = Render.SktSolver.addNewSketchRelation(
            Sel.getSelectedPointIds(), cad::rel_horizontal);

        if (success)
          Render.flags.set(render_update_sketch);
      }

      if (ImGui::Button("Coincident")) {

        bool success = Render.SktSolver.addNewSketchRelation(
            Sel.getSelectedPointIds(), cad::rel_coincident);

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

  ImGui::SetNextWindowPos({320, 20});
  ImGui::SetNextWindowSize({300.0f, 300.0f});

  ImGui::Begin("Grid Options Menu");

  ImGui::InputInt("Grid Size", &Sketch.mGrid.size);
  ImGui::InputFloat("Grid Spacing", &Sketch.mGrid.spacing);

  // Prevent too large of a grid from freezing program
  if (Sketch.mGrid.size > 10000)
                Sketch.mGrid.size = 10000;

  // Limit size of grid to protect from undefined behavior
  if (Sketch.mGrid.spacing > 10000)
              Sketch.mGrid.spacing = 10000;

  if (ImGui::Button("OK")) {
    Render.flags.set(render_update_sketch);
    flags.reset(gui_sketch_grid_menu);
  }

  ImGui::End();
}

void showDebugWindow(sketch::Sketch &Sketch, CADRender &Render,
                     sel::Selector &Sel,
                     std::bitset<gui_num_flags> &flags) {

  ImGui::Begin("Debugging");


  if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_None)) {

    if (Render.debugUtilsSupported)
      ImGui::Text("Debug Utils Supported!");
    
    if (Render.vkSetDebugUtilsObjectNameEXT)
      ImGui::Text("vkSetDebugUtilsObjectNameEXT");

    // Copy of selection point color to check for changes
    glm::vec4 selPointColor = Render.mRenderColors.selPointColor;

    ImGui::ColorPicker4("Background Color", (float *)&Render.mRenderColors.bgColor);
    ImGui::ColorPicker4("Selection Point Color",
                        (float *)&Render.mRenderColors.selPointColor);
    ImGui::ColorPicker4("Sketch Point Color",
                        (float *)&Render.mRenderColors.sketchPointColor);
    ImGui::ColorPicker4("Sketch Line Color",
                        (float *)&Render.mRenderColors.sketchLineColor);
    ImGui::ColorPicker4("Sketch Grid Color",
                        (float *)&Render.mRenderColors.sketchGridColor);

    // Update rendering if selection point color has changed
    if (selPointColor != Render.mRenderColors.selPointColor) {
      Render.flags.set(render_update_sketch);
      std::cout << "Selection Point Color has changed!" << std::endl;
    }
  }

  if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None)) {

    static bool orthoCheck = true;
    ImGui::Checkbox("Orthogonal", &orthoCheck);

    if (orthoCheck && !Render.Cam.flags.test(cam::ortho)) {

      Render.Cam.flags.set(cam::ortho);

    } else if (!orthoCheck && Render.Cam.flags.test(cam::ortho)) {

      Render.Cam.flags.reset(cam::ortho);
    }

    if (ImGui::Button("Set XY View")) {
      Render.Cam.setXYView();
    }
    if (ImGui::Button("Set ZX View")) {
      Render.Cam.setZXView();
    }
    if (ImGui::Button("Set YZ View")) {
      Render.Cam.setYZView();
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

    ImGui::NewLine();

    ImGui::Text("Up");
    ImGui::Text("x: %f", Render.Cam.up.x);
    ImGui::Text("y: %f", Render.Cam.up.y);
    ImGui::Text("z: %f", Render.Cam.up.z);

    ImGui::NewLine();

    ImGui::Text("Cross");
    ImGui::Text("x: %f", Render.Cam.cross.x);
    ImGui::Text("y: %f", Render.Cam.cross.y);
    ImGui::Text("z: %f", Render.Cam.cross.z);
  }

  if (ImGui::CollapsingHeader("Selection", ImGuiTreeNodeFlags_None)) {

    ImGui::Text("Selection Flags");
    ImGui::Text("First Click: %d",
                Sel.flags.test(sel::select_first_click));
    ImGui::Text("Single Point: %d",
                Sel.flags.test(sel::select_single_point));
    ImGui::Text("Double Point: %d",
                Sel.flags.test(sel::select_double_point));
    ImGui::Text("Multi Point: %d",
                Sel.flags.test(sel::select_multi_point));

    ImGui::NewLine();

    ImGui::Text("Selection Point");
    ImGui::Text("x: %f", Sel.point.x);
    ImGui::Text("y: %f", Sel.point.y);
    ImGui::Text("z: %f", Sel.point.z);

    ImGui::NewLine();

    ImGui::Text("Selected Points");
    for (const auto &p : Sel.selectedPoints) {
      ImGui::Text("Selected Point: %d", p.first);
      ImGui::Text("x: %f", p.second.pos.x);
      ImGui::Text("y: %f", p.second.pos.y);
      ImGui::Text("z: %f", p.second.pos.z);
      ImGui::NewLine();
    }
  }

  if (ImGui::CollapsingHeader("Sketch", ImGuiTreeNodeFlags_DefaultOpen)) {

    ImGui::Text("Sketch Tools:");

    ImGui::Text("FeatureCounter: %d", Sketch.getFeatureCounter());

    ImGui::Text("Camera Distance");
    ImGui::Text("Distance: %f", *Sketch.mCamDistance);

    ImGui::NewLine();

    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
        ImGuiTableFlags_Hideable | ImGuiTableFlags_RowBg;

    if (ImGui::BeginTable("Sketch Flags Debug", 2, flags)) {

      ImGui::TableSetupColumn("Flag Name");
      ImGui::TableSetupColumn("Status");
      ImGui::TableHeadersRow();

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Sketch Active");
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", Sketch.flags.test(sketch::skt_active));

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Sketch Points Have Moved");
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", Sketch.flags.test(sketch::skt_points_have_moved));

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Tool Active");
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", Sketch.flags.test(sketch::skt_tool_active));

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Point Tool Active");
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", Sketch.flags.test(sketch::skt_point_tool));

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::Text("Note Tool Active");
      ImGui::TableSetColumnIndex(1);
      ImGui::Text("%d", Sketch.flags.test(sketch::skt_note_tool));

      ImGui::EndTable();
    }

    for (auto &rel : Sketch.mRelations) {

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
      for (const auto &id : rel.second.mFeatureIds) {
        ImGui::Text("%d", id);
      }
    }

    // Sketch Points

    if (ImGui::BeginTable("Sketch Points Debug", 4, flags)) {

      ImGui::TableSetupColumn("Id");
      ImGui::TableSetupColumn("x");
      ImGui::TableSetupColumn("y");
      ImGui::TableSetupColumn("z");
      ImGui::TableHeadersRow();

      for (const auto &point : Sketch.Points) {

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%d", point.first);

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%f", point.second.pos.x);

        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%f", point.second.pos.y);

        ImGui::TableSetColumnIndex(3);
        ImGui::Text("%f", point.second.pos.z);
      }

      ImGui::EndTable();
    }

    ImGui::NewLine();
  }

  ImGui::End();
}

void imguiRun(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel) {

  static std::bitset<gui_num_flags> flags(gui_start_menu + 1);

  // Cadera imgui begin, not part of Dear Imgui
  imguiBegin();

  mainMenuBar();

  if (flags.test(gui_start_menu))
    startMenu(Sketch, Render, Sel, flags);
  if (flags.test(gui_sketch_menu))
    sketchMenu(Sketch, Render, Sel, flags);
  if (flags.test(gui_sketch_grid_menu))
    gridMenu(Sketch, Render, Sel, flags);

  // Cadera imgui end, not part of Dear Imgui
  imguiEnd();
}

} // namespace gui
} // namespace CADERA_APP_NAMESPACE