cmake_minimum_required(VERSION 3.5)





add_library(inputlib ${CMAKE_CURRENT_LIST_DIR}/input.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/keyboard.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/mouse.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/command.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/sketchcommands.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/rendercommands.cpp
                             ${CMAKE_CURRENT_LIST_DIR}/cameracommands.cpp
                             )

target_include_directories(inputlib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/)

target_link_libraries(inputlib PUBLIC renderlib)