#include "Nodes.hpp"

void NodeTest::show()
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



