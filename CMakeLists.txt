cmake_minimum_required(VERSION 3.5)


project(Cadera VERSION 0.1.2)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


enable_testing()

#=========================================
# Targets
#=========================================
add_executable(CaderaApp    src/Cadera.cpp
                            src/Selection.cpp
                            src/callbacks.cpp
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

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include/)

#=========================================
# Libraries
#=========================================


find_package(Vulkan REQUIRED)
target_link_libraries(CaderaApp PRIVATE Vulkan::Vulkan)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE imgui::imgui)

find_package(glm CONFIG REQUIRED)
target_link_libraries(CaderaApp PRIVATE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(CaderaApp PRIVATE ${Stb_INCLUDE_DIR})

#=========================================
# Textures/Shaders
#=========================================

# Copy textures/*.png to build directory
## Make textures/ directory
add_custom_command(
        TARGET CaderaApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/textures/
        )

## Gather list of all .png files in /shaders/
file(GLOB TextureFiles ${CMAKE_SOURCE_DIR}/textures/*.png
                      ${CMAKE_SOURCE_DIR}/textures/*.csv)

foreach(TextureFile ${TextureFiles})
  add_custom_command(TARGET CaderaApp POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${TextureFile} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/textures/)
endforeach()
               

# Copy shaders/*.spv to build directory
## Make shaders/ directory
add_custom_command(
        TARGET CaderaApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders/
        )

## Gather list of all .spv files in /shaders/
file(GLOB ShaderFiles ${CMAKE_SOURCE_DIR}/shaders/*.spv)

foreach(ShaderFile ${ShaderFiles})
  add_custom_command(TARGET CaderaApp POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy ${ShaderFile} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/shaders/)
endforeach()

# Compile Shaders

if(WIN32)
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/shaders/compile_shaders.bat ${CMAKE_SOURCE_DIR}/shaders/)
endif(WIN32)

if(UNIX)
    execute_process(COMMAND bash ${CMAKE_SOURCE_DIR}/shaders/compile_shaders.sh ${CMAKE_SOURCE_DIR}/shaders/)
endif(UNIX)

#=========================================
# Testing
#=========================================
add_subdirectory(test)



#=========================================
# Packaging
#=========================================
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
install(TARGETS CaderaApp RUNTIME DESTINATION bin)
include(CPack)
