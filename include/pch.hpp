#pragma once

#include <cassert>
#include <iostream>
#include <map>
#include <vector>
#include <bitset>
#include <list>
#include <set>



#define CADERA_APP_NAMESPACE cad
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN

#ifdef _DEBUG
#define CADERA_DEBUG
#endif

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "GLFW/glfw3.h"