#pragma once

// STL
#include <bitset>
#include <cassert>
#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>

// Project Namespaces
#define CADERA_APP_NAMESPACE cad

// Windows MINGW64 gcc/MSVC
#ifdef _DEBUG
#define CADERA_DEBUG
#endif

// Linux gcc clang
#ifndef NDEBUG
#define CADERA_DEBUG
#endif

// Vulkan
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

// GLFW3
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

// Dear ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui_stdlib.h>

// Project Includes
#include "config.hpp"