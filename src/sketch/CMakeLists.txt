cmake_minimum_required(VERSION 3.5)





add_library(sketchlib ${CMAKE_CURRENT_LIST_DIR}/Relation.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/Feature.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/Sketch.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/Sketcher.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/SketchSolver.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/Point.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/Model.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/grid.cpp
                             )

target_include_directories(sketchlib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/)
target_include_directories(sketchlib PRIVATE ${CMAKE_SOURCE_DIR}/src/text/)

target_link_libraries(sketchlib PUBLIC pch_interface)


target_link_libraries(sketchlib PRIVATE textlib)

find_package(glm CONFIG REQUIRED)
target_link_libraries(sketchlib PRIVATE glm::glm)