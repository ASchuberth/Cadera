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


# Test_Camera
add_executable(Test_Cadera_Sketch test_sketch.cpp)

target_include_directories(Test_Cadera_Sketch PRIVATE ${CMAKE_SOURCE_DIR}/src)


target_link_libraries(Test_Cadera_Sketch PRIVATE sketchlib)
target_link_libraries(Test_Cadera_Sketch INTERFACE pch_interface)

target_include_directories(Test_Cadera_Sketch PRIVATE ${GTEST_INCLUDE_DIRS} )                               
target_link_libraries(Test_Cadera_Sketch PRIVATE ${GTEST_BOTH_LIBRARIES})


# Test_Text
add_executable(Test_Cadera_Text test_text.cpp)
target_include_directories(Test_Cadera_Text PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(Test_Cadera_Text PRIVATE textlib)
target_link_libraries(Test_Cadera_Text INTERFACE pch_interface)

target_include_directories(Test_Cadera_Text PRIVATE ${GTEST_INCLUDE_DIRS} )                               
target_link_libraries(Test_Cadera_Text PRIVATE ${GTEST_BOTH_LIBRARIES})



include(GoogleTest)
gtest_discover_tests(Test_Cadera_Sketch)
gtest_discover_tests(Test_Cadera_Text)
