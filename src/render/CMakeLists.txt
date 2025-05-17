cmake_minimum_required(VERSION 3.5)

add_library(renderlib ${CMAKE_CURRENT_LIST_DIR}/cadrender.cpp
                      ${CMAKE_CURRENT_LIST_DIR}/camera.cpp
                      ${CMAKE_CURRENT_LIST_DIR}/selection.cpp
                                 )


target_link_libraries(renderlib PUBLIC sketchlib)
target_link_libraries(renderlib PUBLIC textlib)