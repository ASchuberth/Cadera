cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" )
project(Cadera VERSION 0.1.0)

include(CTest)
enable_testing()



add_executable(Cadera   src/Cadera.cpp
                        src/CADRender.cpp  
                        src/TextRender.cpp  
                        src/RenderUtil.cpp  
                        src/Selection.cpp
                        src/Camera.cpp   
                        src/Callbacks.cpp
                        src/gui.cpp
                        src/Sketch.cpp
                        src/Sketcher.cpp
                        src/SketchSolver.cpp
                        src/Point.cpp
                        src/Model.cpp
                        src/Feature.cpp
                        src/Relation.cpp
                        src/grid.cpp      
                        src/Main.cpp
                       
                      
)
    

add_executable(tester test/tester.cpp)
add_test(tester tester)

target_precompile_headers(Cadera PRIVATE include/pch.hpp)
target_include_directories(Cadera PRIVATE include/)

find_package(Vulkan REQUIRED)
target_link_libraries(Cadera PRIVATE Vulkan::Vulkan)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Cadera PRIVATE glfw)

find_package(Stb REQUIRED)
target_include_directories(Cadera PRIVATE ${Stb_INCLUDE_DIR})

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(Cadera PRIVATE Microsoft.GSL::GSL)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Cadera PRIVATE imgui::imgui)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
