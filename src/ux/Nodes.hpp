#pragma once

#include <imgui.h>
#include "imnodes/imnodes_internal.h"
#include "nodeeditor/imgui_node_editor.h"

namespace ed = ax::NodeEditor;


class NodeTest {
public:
    void showImNodes();
   
    void showNodeEditor(ax::NodeEditor::EditorContext* pNodeEditorContext);

};
