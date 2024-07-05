cmake_minimum_required(VERSION 3.13)
set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" )
project(Cadera VERSION 0.1.0)

include(CTest)
enable_testing()

#=========================================
# Targets
#=========================================
add_executable(CaderaApp    src/Cadera.cpp
                            src/Selection.cpp
                            src/Callbacks.cpp
                            src/Model.cpp    
                            src/Main.cpp
                                     
)


add_subdirectory(src/render)
add_subdirectory(src/sketch)
add_subdirectory(src/ux)

set_target_properties(CaderaApp PROPERTIES
    OUTPUT_NAME CaderaApp
    EXPORT_NAME CaderaApp
)

target_compile_definitions(CaderaApp PUBLIC)

#=========================================
# Precompiled Headers / Include Directories
#=========================================
target_precompile_headers(CaderaApp PRIVATE src/pch.hpp)

target_include_directories(CaderaApp PRIVATE src/)

#=========================================
# Libraries
#=========================================
find_package(Vulkan REQUIRED)
target_link_libraries(CaderaApp PRIVATE Vulkan::Vulkan)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE glfw)

find_package(Stb REQUIRED)
target_include_directories(CaderaApp PRIVATE ${Stb_INCLUDE_DIR})

find_package(Microsoft.GSL CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE Microsoft.GSL::GSL)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE imgui::imgui)


#=========================================
# Testing
#=========================================
include_directories(test)



#=========================================
# Packaging
#=========================================
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
