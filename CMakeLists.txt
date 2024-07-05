cmake_minimum_required(VERSION 3.14)
set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" )
project(Cadera VERSION 0.1.2)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

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

# find_package(glm CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE glm::glm)

#=========================================
# Testing
#=========================================
add_subdirectory(test)



#=========================================
# Packaging
#=========================================
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
