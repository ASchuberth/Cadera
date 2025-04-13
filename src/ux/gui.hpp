#pragma once

#include "../render/CADRender.hpp"
// #include "Nodes.hpp"
#include "Events.hpp"



namespace CADERA_APP_NAMESPACE {
namespace gui {

	enum GUIFlags {
		gui_start_menu,
		gui_sketch_menu,
		gui_sketch_grid_menu,
		gui_sketch_node_editor,
		gui_num_flags
	};

	void imguiRun(Events &CadEvents, sketch::Sketch &Sketch, CADRender &Render, sel::Selector& Sel);

	void imguiBegin(ax::NodeEditor::EditorContext *& NodeEditorContext);

	void imguiEnd();

	void mainMenuBar();

	void startMenu(sketch::Sketch& Sketch, CADRender &Render, sel::Selector &Sel, 
		           std::bitset<gui_num_flags> &flags);

	void sketchMenu(sketch::Sketch& Sketch, CADRender &Render, sel::Selector &Sel, 
		            std::bitset<gui_num_flags> &flags);
	
	void gridMenu(sketch::Sketch &Sketch, CADRender &Render, sel::Selector &Sel, 
		            std::bitset<gui_num_flags> &flags);

	void nodeMenu(CADRender &Render);
	
	void showDebugWindow(sketch::Sketch& Sketch, CADRender &Render, sel::Selector &Sel, 
		                 std::bitset<gui_num_flags> &flags);

}
}