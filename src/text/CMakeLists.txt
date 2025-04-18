cmake_minimum_required(VERSION 3.5)

add_library(textlib ${CMAKE_CURRENT_LIST_DIR}/TextRender.cpp)
                             

target_link_libraries(textlib PUBLIC pch_interface)

target_include_directories(textlib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)

find_package(glm CONFIG REQUIRED)
target_link_libraries(textlib PRIVATE glm::glm)