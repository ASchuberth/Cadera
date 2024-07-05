cmake_minimum_required(VERSION 3.14)


add_executable(GTest_Cadera test_sketch.cpp)



target_precompile_headers(GTest_Cadera PRIVATE pch.hpp)
target_include_directories(GTest_Cadera PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_sources(GTest_Cadera PRIVATE ../src/sketch/Relation.cpp
                                    ../src/sketch/Feature.cpp
                                    ../src/sketch/Point.cpp
                                    )


target_link_libraries(GTest_Cadera PRIVATE GTest::gtest_main)

target_link_libraries(GTest_Cadera PRIVATE glm::glm)

include(GoogleTest)
gtest_discover_tests(GTest_Cadera)



# target_link_libraries(GTest_Cadera PRIVATE Vulkan::Vulkan)
# target_link_libraries(GTest_Cadera PRIVATE glfw)
# target_include_directories(GTest_Cadera PRIVATE ${Stb_INCLUDE_DIR})
# target_link_libraries(GTest_Cadera PRIVATE Microsoft.GSL::GSL)
# target_link_libraries(GTest_Cadera PRIVATE imgui::imgui)

enable_testing()
