cmake_minimum_required(VERSION 3.5)





add_library(sketchlib ${CMAKE_CURRENT_LIST_DIR}/relation.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/feature.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/sketch.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/sketcher.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/sketchsolver.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/point.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/model.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/grid.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/observer.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/subject.cpp
                             )

target_include_directories(sketchlib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/)
target_include_directories(sketchlib PRIVATE ${CMAKE_SOURCE_DIR}/src/text/)

target_link_libraries(sketchlib PUBLIC pch_interface)


target_link_libraries(sketchlib PRIVATE textlib)

find_package(glm CONFIG REQUIRED)
target_link_libraries(sketchlib PRIVATE glm::glm)