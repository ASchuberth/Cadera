cmake_minimum_required(VERSION 3.5)


target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/gui.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/actions/action.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/actions/action_queue.cpp
                                 )

                                

                              