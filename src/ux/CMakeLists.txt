cmake_minimum_required(VERSION 3.5)


target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/gui.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/command/command.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/command/mouse.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/command/sketchcommands.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/command/cameracommands.cpp
                                 )

                                

                              