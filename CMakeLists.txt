cmake_minimum_required(VERSION 3.5)


project(Cadera VERSION 0.1.2)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") 


enable_testing()

#=========================================
# Targets
#=========================================
add_executable(CaderaApp    src/Cadera.cpp
                            src/Selection.cpp
                            src/callbacks.cpp
                            src/Main.cpp  
)


set_target_properties(CaderaApp PROPERTIES
    OUTPUT_NAME CaderaApp
    EXPORT_NAME CaderaApp
)

target_compile_definitions(CaderaApp PUBLIC)

#=========================================
# Precompiled Headers / Include Directories
#=========================================

add_library(pch_interface INTERFACE)
target_precompile_headers(pch_interface INTERFACE src/pch.hpp) # pch.hpp is the precompiled header
target_include_directories(pch_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/) # Include directory for pch.h



target_include_directories(CaderaApp PRIVATE src/)

add_subdirectory(src/render)
add_subdirectory(src/sketch)
add_subdirectory(src/text)
add_subdirectory(src/ux)


#=========================================
# Libraries
#=========================================



find_package(Vulkan REQUIRED)
target_link_libraries(pch_interface INTERFACE Vulkan::Vulkan)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(pch_interface INTERFACE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(pch_interface INTERFACE imgui::imgui)

find_package(glm CONFIG REQUIRED)
target_link_libraries(pch_interface INTERFACE glm::glm)

find_package(Stb REQUIRED)
target_include_directories(pch_interface INTERFACE ${Stb_INCLUDE_DIR})

target_link_libraries(CaderaApp PRIVATE sketchlib)
target_link_libraries(CaderaApp PRIVATE textlib)

target_link_libraries(CaderaApp PUBLIC pch_interface)

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
#add_subdirectory(test)



#=========================================
# Packaging
#=========================================
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
install(TARGETS CaderaApp RUNTIME DESTINATION bin)
include(CPack)
