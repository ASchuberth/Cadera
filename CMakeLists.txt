cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE "C:\\Users\\amsch\\Documents\\Programming\\Cpp\\vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" )
project(Cadera VERSION 0.1.0)

include(CTest)
enable_testing()



add_executable(Cadera src/Main.cpp
                        src/Cadera.cpp
                        src/callbacks.cpp)


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

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
