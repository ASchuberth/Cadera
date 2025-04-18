cmake_minimum_required(VERSION 3.5)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



include(CTest)

find_package(GTest REQUIRED)

add_executable(GTest_Cadera test_sketch.cpp
                            test_camera.cpp
)



target_precompile_headers(GTest_Cadera PRIVATE pch.hpp)
target_include_directories(GTest_Cadera PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_sources(GTest_Cadera PRIVATE ../src/sketch/Relation.cpp
                                    ../src/sketch/Feature.cpp
                                    ../src/sketch/Point.cpp
                                    ../src/sketch/Sketcher.cpp
                                    ../src/sketch/Sketch.cpp
                                    ../src/sketch/SketchSolver.cpp
                                    ../src/render/grid.cpp
                                    ../src/render/RenderUtil.cpp
                                    ../src/render/CADRender.cpp
                                    ../src/render/TextRender.cpp
                                    ../src/render/Camera.cpp
                                    ../src/Selection.cpp
                                    ../src/Model.cpp
                                    ../src/callbacks.cpp
                                    ../src/Cadera.cpp
                                    ../src/ux/gui.cpp
                                    ../src/ux/Nodes.cpp
                                    # ../src/ux/imnodes/imnodes.cpp
                                    )


target_include_directories( GTest_Cadera PRIVATE ${GTEST_INCLUDE_DIRS} )                               
target_link_libraries(GTest_Cadera PRIVATE ${GTEST_BOTH_LIBRARIES})
target_link_libraries(GTest_Cadera PRIVATE glm::glm)
find_package(Vulkan REQUIRED)
target_link_libraries(GTest_Cadera PRIVATE Vulkan::Vulkan)
target_link_libraries(GTest_Cadera PRIVATE glfw)
target_include_directories(GTest_Cadera PRIVATE ${Stb_INCLUDE_DIR})
find_package(imgui CONFIG REQUIRED)
target_link_libraries(GTest_Cadera PRIVATE imgui::imgui)


include(GoogleTest)
gtest_discover_tests(GTest_Cadera)
