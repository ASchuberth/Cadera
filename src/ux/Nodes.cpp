#include "Nodes.hpp"



void NodeTest::showImNodes()
{
    ImNodes::CreateContext();
    ImNodes::SetNodeGridSpacePos(1, ImVec2(200.0f, 200.0f));

    ImGui::Begin("simple node editor");

    ImNodes::BeginNodeEditor();
    ImNodes::BeginNode(1);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("Node 1");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(2);
    ImGui::Text("input");
    ImNodes::EndInputAttribute();

    static float nodeValue = 0.0f;

    ImGui::PushItemWidth(30.f);
    ImGui::InputFloat("Value", &nodeValue);
    ImGui::PopItemWidth();

    ImNodes::BeginOutputAttribute(3);
    ImGui::Indent(50);
    ImGui::Text("output");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();



    ImNodes::BeginNode(4);

    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("Node 2");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(5);
    ImGui::Text("input");
    ImNodes::EndInputAttribute();

    static float nodeValue2 = 0.0f;

    ImGui::PushItemWidth(50.f);
    ImGui::InputFloat("Value", &nodeValue2);
    ImGui::PopItemWidth();

    ImNodes::BeginOutputAttribute(6);
    ImGui::Indent(40);
    ImGui::Text("output");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();


    ImNodes::EndNodeEditor();

    ImGui::End();
    
}

void NodeTest::showNodeEditor(ax::NodeEditor::EditorContext* pNodeEditorContext) {

    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    //ImGui::Separator();

    ed::SetCurrentEditor(pNodeEditorContext);
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    int uniqueId = 1;
    // Start drawing nodes.
    ed::BeginNode(uniqueId++);
        ImGui::Text("Node A");
        ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
        ed::EndPin();
    ed::EndNode();
    ed::End();
    ed::SetCurrentEditor(nullptr);


}
