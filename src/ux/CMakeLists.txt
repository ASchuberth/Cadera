cmake_minimum_required(VERSION 3.14)


target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/gui.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/Nodes.cpp
                                 )

target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/imnodes/imnodes.cpp)
                                

target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/nodeeditor/imgui_node_editor.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/nodeeditor/imgui_canvas.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/nodeeditor/imgui_node_editor_api.cpp 
                                 ${CMAKE_CURRENT_LIST_DIR}/nodeeditor/crude_json.cpp 
                                 )

                            