cmake_minimum_required(VERSION 3.5)


project(Cadera VERSION 0.1.2)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") 


enable_testing()

#=========================================
# Targets
#=========================================
add_executable(CaderaApp    src/cadera.cpp
                            src/selection.cpp
                            src/callbacks.cpp
                            src/main.cpp  
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
target_precompile_headers(pch_interface INTERFACE src/pch.hpp) 
target_include_directories(pch_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/) 



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

# Precompiled Headers
target_link_libraries(CaderaApp PUBLIC pch_interface)

#=========================================
# Textures/Shaders
#=========================================

get_filename_component(VERT_SHADER_FILE_PATH            ${CMAKE_SOURCE_DIR}/shaders/vert.spv     ABSOLUTE)
get_filename_component(FRAG_SHADER_FILE_PATH            ${CMAKE_SOURCE_DIR}/shaders/frag.spv     ABSOLUTE)
get_filename_component(GRID_VERTEX_SHADER_FILE_PATH     ${CMAKE_SOURCE_DIR}/shaders/gridvert.spv ABSOLUTE)
get_filename_component(TEXT_VERTEX_SHADER_FILE_PATH     ${CMAKE_SOURCE_DIR}/shaders/textvert.spv ABSOLUTE)
get_filename_component(TEXT_FRAGMENT_SHADER_FILE_PATH   ${CMAKE_SOURCE_DIR}/shaders/textfrag.spv ABSOLUTE)

get_filename_component(ARIAL_FONT_TEXTURE_FILE_PATH     ${CMAKE_SOURCE_DIR}/textures/arial.png    ABSOLUTE)
get_filename_component(ARIAL_FONT_COORDINATES_FILE_PATH ${CMAKE_SOURCE_DIR}/textures/arial.csv    ABSOLUTE)
 
configure_file(${CMAKE_SOURCE_DIR}/src/config.hpp.in ${CMAKE_SOURCE_DIR}/src/config.hpp)


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
