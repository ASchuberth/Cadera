cmake_minimum_required(VERSION 3.5)

target_sources(CaderaApp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/cadrender.cpp
                                 ${CMAKE_CURRENT_LIST_DIR}/camera.cpp
                                 )