cmake_minimum_required(VERSION 3.14)


target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/gui.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/Nodes.cpp
                                 )

target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/imnodes/imnodes.cpp
                                

)                              