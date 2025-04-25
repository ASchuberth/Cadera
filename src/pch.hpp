#pragma once

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

#define CADERA_APP_NAMESPACE cad
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define IMGUI_DEFINE_MATH_OPERATORS

// Windows MINGW64 gcc
#ifdef _DEBUG
#define CADERA_DEBUG
#endif

// Linux gcc
#ifndef NDEBUG
#define CADERA_DEBUG
#endif

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "GLFW/glfw3.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <imgui_stdlib.h>
